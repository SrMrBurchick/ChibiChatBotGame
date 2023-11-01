import asyncio
from WebSockServer.web_sock_server import WebSockServer
from TwitchChatBot.chat_bot import create_bot
import json


CONFIG_FILE = "./Game/config/ChibiChatBotConfig.json"

async def main():
    channel = ""
    # Default commands
    commands = ["say"]

    try:
        with open(CONFIG_FILE) as conf_file:
            conf_data = conf_file.read()
            config = json.loads(conf_data)

            if config is not None:
                if config["twitch-channel"] is not None:
                    channel = config["twitch-channel"]

                for animation in config["animations"]:
                    for key in animation.keys():
                        commands.append(key)
    except:
        pass

    print(commands)
    print(channel)

    wss = WebSockServer()
    bot = create_bot(wss, commands, channel)

    wss_task = wss.run_server()
    loop = asyncio.get_running_loop()
    bot_task = loop.run_in_executor(None, bot.start)

    await asyncio.gather(wss_task, bot_task)

if __name__ == "__main__":
    asyncio.run(main())
