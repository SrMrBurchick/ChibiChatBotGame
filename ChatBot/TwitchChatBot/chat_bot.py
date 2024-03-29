'''
Copyright 2017 Amazon.com, Inc. or its affiliates. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License"). You may not use this file except in compliance with the License. A copy of the License is located at

    http://aws.amazon.com/apache2.0/

or in the "license" file accompanying this file. This file is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.
'''

import irc.bot
import requests
import asyncio
import json
from WebSockServer.web_sock_server import WebSockServer
import logging

import os
import sys
sys.path.insert(0, os.path.abspath(
    os.path.join(os.path.dirname(__file__), '..')))

CREDENTIALS_FILE = "../credentials.json"


class TwitchBot(irc.bot.SingleServerIRCBot):
    def __init__(self, username, client_id, token, channel, wss: WebSockServer, commands: list, any_user: bool):
        self.client_id = client_id
        self.token = token
        self.channel = '#' + channel
        self.user = channel.lower()
        self.wss = wss
        self.commands = commands
        self.any_user = any_user

        # Create IRC bot connection
        server = 'irc.chat.twitch.tv'
        port = 6667
        logging.debug('Connecting to ' + server + ' on port ' + str(port) + '...')
        irc.bot.SingleServerIRCBot.__init__(
            self, [(server, port, 'oauth:'+token)], username, username)

    def on_welcome(self, c, e):
        logging.debug('Joining ' + self.channel)

        # You must request specific capabilities before you can use them
        c.cap('REQ', ':twitch.tv/membership')
        c.cap('REQ', ':twitch.tv/tags')
        c.cap('REQ', ':twitch.tv/commands')
        c.join(self.channel)

    def on_pubmsg(self, c, e):
        valid_user = False
        for i in range(0, len(e.arguments)):
            logging.debug(f"Received message from {e.source.nick} with user level {e.tags}: {e.arguments[i]}")

        if self.any_user:
            valid_user = True
        else:
            valid_user = e.source.nick == self.user
        # If a chat message starts with an exclamation point, try to run it as a command
        if e.arguments[0][:1] == '!' and valid_user:
            cmd = e.arguments[0].split(' ')[0][1:]
            logging.debug('Received command: ' + cmd)
            self.do_command(e, cmd)
        return

    def do_command(self, e, cmd):
        c = self.connection

        if cmd in self.commands:
            # Not best but simplest and fastest solution ever )))
            args = e.arguments[0].split()
            args = args[1:]
            loop = asyncio.new_event_loop()
            asyncio.set_event_loop(loop)
            loop.run_until_complete(self.wss.send_command(cmd, args))
        elif cmd == "commands":
            commands = ""
            blocked_commands = ["fall"]
            for command in self.commands:
                if command not in blocked_commands:
                    commands = "{} {}".format(commands, command)
            c.privmsg(self.channel, "Available commands: " + commands)
            logging.debug("Available commands: " + commands)
        else:
            c.privmsg(self.channel, "Did not understand command: " + cmd)

    async def run_bot(self):
        await self.start()


def create_bot(wss: WebSockServer, commands: list, channel: str, any_user: bool) -> TwitchBot:
    creds_path = os.path.join(sys._MEIPASS, "credentials.json") if hasattr(sys, '_MEIPASS') else CREDENTIALS_FILE
    with open(creds_path) as creds_file:
        creds_data = creds_file.read()

    creds = json.loads(creds_data)
    username = "ChibChatBot"
    client_id = creds["client-id"]
    token = creds["token"]

    return TwitchBot(username, client_id, token, channel, wss, commands, any_user)


if __name__ == "__main__":
    bot = create_bot()
    bot.start()
