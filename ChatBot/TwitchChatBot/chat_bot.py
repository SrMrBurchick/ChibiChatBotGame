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

import os
import sys
sys.path.insert(0, os.path.abspath(
    os.path.join(os.path.dirname(__file__), '..')))

CREDENTIALS_FILE = "../credentials.json"


class TwitchBot(irc.bot.SingleServerIRCBot):
    def __init__(self, username, client_id, token, channel, wss: WebSockServer, commands: list):
        self.client_id = client_id
        self.token = token
        self.channel = '#' + channel
        self.wss = wss
        self.commands = []

        # Create IRC bot connection
        server = 'irc.chat.twitch.tv'
        port = 6667
        print('Connecting to ' + server + ' on port ' + str(port) + '...')
        irc.bot.SingleServerIRCBot.__init__(
            self, [(server, port, 'oauth:'+token)], username, username)

    def on_welcome(self, c, e):
        print('Joining ' + self.channel)

        # You must request specific capabilities before you can use them
        c.cap('REQ', ':twitch.tv/membership')
        c.cap('REQ', ':twitch.tv/tags')
        c.cap('REQ', ':twitch.tv/commands')
        c.join(self.channel)

    def on_pubmsg(self, c, e):
        for i in range(0, len(e.arguments)):
            print("Recived message: " + e.arguments[i])

        # If a chat message starts with an exclamation point, try to run it as a command
        if e.arguments[0][:1] == '!':
            cmd = e.arguments[0].split(' ')[0][1:]
            print('Received command: ' + cmd)
            self.do_command(e, cmd)
        return

    def do_command(self, e, cmd):
        c = self.connection

        if cmd in self.commands:
            loop = asyncio.new_event_loop()
            asyncio.set_event_loop(loop)
            loop.run_until_complete(self.wss.send_command(cmd, None))
        elif cmd == "commands":
            commands = ""
            for command in self.commands:
                commands += "{command}\n"
            c.privmsg(self.channel, "Available commands: " + commands)
        else:
            c.privmsg(self.channel, "Did not understand command: " + cmd)

    async def run_bot(self):
        await self.start()


def create_bot(wss: WebSockServer, commands: list) -> TwitchBot:
    with open(CREDENTIALS_FILE) as creds_file:
        creds_data = creds_file.read()

    creds = json.loads(creds_data)
    username = "ChibiGameChatBot"
    client_id = creds["client-id"]
    token = creds["token"]
    channel = "SrMrBurchick"

    return TwitchBot(username, client_id, token, channel, wss, commands)


if __name__ == "__main__":
    bot = create_bot()
    bot.start()
