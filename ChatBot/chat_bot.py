import asyncio
from WebSockServer.web_sock_server import WebSockServer
from TwitchChatBot.chat_bot import create_bot


async def main():
    # TODO add config and credentials reading
    wss = WebSockServer()
    bot = create_bot()

    wss_task = wss.run_server()
    loop = asyncio.get_running_loop()
    bot_task = loop.run_in_executor(None, bot.start)

    await asyncio.gather(wss_task, bot_task)

if __name__ == "__main__":
    asyncio.run(main())
