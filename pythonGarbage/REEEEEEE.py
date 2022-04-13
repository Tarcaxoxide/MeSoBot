#!/usr/bin/python3.9
import socket
import asyncio
from mi import *
import time
import Credential
time.sleep(10)
IP=Credential.ip
PORT=Credential.port
ADDR=(IP,PORT)
print(f'Connecting to socket {ADDR}...')
zclient = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
zclient.connect(ADDR)

uri=Credential.uri
token=Credential.token
channels=Credential.channels


MSGZ=""
async def send(msg):
    RETURN_MESSAGE='?'
    zclient.send(msg.encode('utf-8'))
    MSGZ= b'\0'*4096
    MSGZ=zclient.recv(4096).decode('utf-8')
    time.sleep(1)
    print(f'-{MSGZ}-')
    return MSGZ

class MyBot(commands.Bot):
    def __init__(self):
        super().__init__()

    @tasks.loop(60)
    async def task(self):
        print('loop')

    async def on_ready(self, ws):
        print("work on my machine")
        await Router(ws).connect_channel(channels)  # globalとmainチャンネルに接続
        #self.task.start()  # タスクを起動する
        #self.task.stop()  # タスクを止める

    async def on_message(self, note: Note):
        instance_name = note.author.instance.name if note.author.instance else 'local'
        username = note.author.nickname or note.author.name
        MSGZ = await send(note.content)
        res = await note.reply(str(MSGZ))
        print(f'?{MSGZ}?')
        ##if note.renote is None:
        ##    print(f'{instance_name} | {username}: {note.content}')
        ##else:
        ##    renote_name = note.renote.user.name
        ##    print(f'{instance_name} | {username}: {note.content}\n    {renote_name}: {note.renote.content}')


if __name__ == "__main__":
    asyncio.run(MyBot().start(uri, token))
