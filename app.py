#!/usr/bin/python3.9
import asyncio

from mi import *
import importlib
import json

import Brain

Channels = []


class MyBot(commands.Bot):
    def __init__(self):
        super().__init__()

    @tasks.loop(60)
    async def task(self):
        print("I'm Looping!")

    async def on_ready(self, ws):
        print("work on my machine")
        await Router(ws).connect_channel(Channels) # channels to connect to
        self.task.start()  # Launch a task
        ##res = await self.client.note.send('hello world')
        ##print(res.content)
        self.task.stop()  # Stop a task

    async def on_message(self, note: Note):
        importlib.reload(Brain)
        instance_name = note.author.instance.name if note.author.instance else 'local'
        username = note.author.nickname or note.author.name

        message_reply = await Brain.cmd(note.author.name,note.content,instance_name,self)
        if message_reply != 'NULL':
            res = await note.reply('%s' % message_reply)
            


if __name__ == "__main__":
    with open('../MeSoBot_Data.json') as Data_File:
        Data = json.load(Data_File)
        uri = Data['uri']
        token = Data['token']
        Channels = Data['Channels']

        asyncio.run(MyBot().start(uri, token))
