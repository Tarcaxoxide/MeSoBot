import asyncio
from mi import *

from subprocess import PIPE, run
import json
import time;

BOT_NAME='@MeSoBot'

async def Log(text):
    string_log=f'\n[{time.asctime( time.localtime(time.time()) )}]:{text}'
    f = open("Access.Log.txt", "a")
    f.write(string_log)
    print(string_log)
    f.close()

async def Allowed(cmdd):
    #parse the data file to find out if we are allowed to run the command in question.
    with open('MeSoBot_Data.json') as Data_File:
        Data = json.load(Data_File)
        for c in Data['AllowedShellCommands']:
            if cmdd == c:
                return True
    return False

async def shell(_Input):
    command=str(f'shelldir/{_Input[0].strip()}.sh')
    command.strip()
    _command=_Input[0].strip()
    if await Allowed(_command): #check to see if we are allowed to run this command before we even parse what the command is.
                                    #this prevents people from executing commands that still work but are potentially a security hole.
                                    #unless your stupid enough to add those to the allowed commands in the data file.
        for sarg in _Input[1:]: 
            command=command+' '+sarg # we compile the command to be run
        _Output = run(command, stdout=PIPE, stderr=PIPE, universal_newlines=True, shell=True) #we run the command and get the output.
    else:
        return f'${_command} Not Allowed!' # if we were not allowed to run this command or it does not exist then we tell the user this.
    return _Output.stdout+_Output.stderr

async def cmd(author,message,instance,bot_ref):
    reply='NULL'
    args = str(message).split(' ')
    
    if args[0] == BOT_NAME: #only execute if the bot is tagged at the start of the post 
        shellargs=str('')
        for sarg in args[1:]:
            shellargs=shellargs+sarg+str(' ')
        print(f'shell {shellargs}')
        reply = await shell(shellargs.split(' '))
        string_log=str(f'[{reply.strip()}] -> @{author}@{instance}')
        Log(string_log)
        
    return reply

