import asyncio
import websockets
import json

class WebSockServer:
    def __init__(self):
        self.__websocket = None
        self.__port = 6565 # Default port. TODO: add config reading
        self.__url = "localhost"

    async def __handle_server__(self, websocket):
        self.__websocket = websocket
        try:
            print("WSS: Client connected")
            async for message in self.__websocket:
                print(f'WSS: Get new message: {message}')

        finally:
            print("WSS: Client disconnected")
            self.__websocket = None

    def is_client_connected(self) -> bool:
        return self.__websocket is not None

    def send_command(self, command: str, author: str):
        if self.is_client_connected():
            event = {
                "action": command
            }

            if author is not None:
                event["author"] = author

            self.__websocket.send(json.dumps(event))

    async def run_server(self):
        print("Start Web Socket Server")
        async with websockets.serve(self.__handle_server__, self.__url, self.__port):
            await asyncio.Future()  # run forever

async def main():
    wss = WebSockServer()
    wss_task = wss.run_server()
    await asyncio.gather(wss_task)

if __name__ == "__main__":
    asyncio.run(main())
