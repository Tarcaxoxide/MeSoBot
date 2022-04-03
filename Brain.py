import asyncio
from mi import *

from subprocess import PIPE, run
import json


BOT_NAME='@MeSoBot'

CommandsAllowed = ('calculate','date')

async def Allowed(cmdd):
    with open('../MeSoBot_Data.json') as Data_File:
        Data = json.load(Data_File)
        for c in Data['AllowedShellCommands']:
            if cmdd == c:
                return True
    return False

async def shell(_Input):
    command=str(f'shelldir/{_Input[0].strip()}.sh')
    command.strip()
    _command=_Input[0].strip()
    if await Allowed(_command):
        for sarg in _Input[1:]:
            command=command+' '+sarg
        _Output = run(command, stdout=PIPE, stderr=PIPE, universal_newlines=True, shell=True)
    else:
        return f'${_command} Not Allowed!'
    return _Output.stdout+_Output.stderr

async def cmd(author,message,instance,bot_ref):
    reply='NULL'
    args = str(message).split(' ')
    

    if args[0] == BOT_NAME:
        shellargs=str('')
        for sarg in args[1:]:
            shellargs=shellargs+sarg+str(' ')
        print(f'shell {shellargs}')
        reply = await shell(shellargs.split(' '))

    
    if reply != 'NULL':
        print(f'[{reply.strip()}] -> @{author}@{instance}')
    return reply

