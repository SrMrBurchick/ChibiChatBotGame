import asyncio
import websockets
import json
import logging

class WebSockServer:
    def __init__(self, url, port):
        self.__websocket = None
        self.__port = port
        self.__url = url

    async def __handle_server__(self, websocket):
        self.__websocket = websocket
        try:
            print("WSS: Client connected")
            async for message in self.__websocket:
                print(f'WSS: Get new message: {message}')
                await self.send_command(message, None)

        finally:
            print("WSS: Client disconnected")
            self.__websocket = None

    def is_client_connected(self) -> bool:
        return self.__websocket is not None

    async def send_command(self, command: str, arguments: list):
        print(f'Received {command} with {arguments}')
        if self.is_client_connected():
            event = {
                "action": command
            }

            # print(e.arguments)
            if command == "say":
                event["message"] = " ".join(arguments)

            logging.debug(f'Send command to client: {event}')
            await self.__websocket.send(json.dumps(event))

    async def run_server(self):
        print("Start Web Socket Server")
        await websockets.serve(self.__handle_server__, self.__url, self.__port)

async def main():
    wss = WebSockServer("localhost", 6565)
    wss_task = wss.run_server()
    await asyncio.gather(wss_task)

if __name__ == "__main__":
    asyncio.run(main())
