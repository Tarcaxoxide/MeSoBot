import asyncio
from mi import *

from subprocess import PIPE, run
import json
import time;

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

async def CheckArgumentCount(cmdd,cmdc):
    with open('MeSoBot_Data.json') as Data_File:
        Data = json.load(Data_File)
        z=0
        for c in Data['AllowedShellCommands']:
            if cmdd == c:
                if Data['ArgumentCountShellCommands'][z] == -1:
                    return True
                if cmdc == Data['ArgumentCountShellCommands'][z]:
                    return True
                else:
                    return False
            z+=1
    return False

async def shell(_Input,user,offset):
    command=str(f'shelldir/{_Input[0].strip()}.sh')
    command.strip()
    _command=_Input[0]
    if await Allowed(_command): #check to see if we are allowed to run this command before we even parse what the command is.
                                    #this prevents people from executing commands that still work but are potentially a security hole.
                                    #unless your stupid enough to add those to the allowed commands in the data file.
        command+=' \''
        command+=_Input[1]
        command+='\''
        argzcount=0
        for sarg in _Input[2:]:
            argzcount+=1
            command=command+' '+sarg # we compile the command to be run
        command+=' \''
        command+=user
        command+='\''
        if await CheckArgumentCount(_command,argzcount):
            _Output = run(command, stdout=PIPE, stderr=PIPE, universal_newlines=True, shell=True) #we run the command and get the output.
        else:
            return f'{argzcount} incorrect number of arguments!'
    else:
        return 'NULL' 
    
    if f'!{_Output.stdout}' != '!':
        _Output.stdout='```\n'+_Output.stdout+'\n```'
        return _Output.stdout
    return 'NULL'

async def BotMentioned(message,BotName):
    message = str(message).replace("\n"," ")
    args = str(message).split(' ')
    trival=0
    for _arg in args:
        trival+=1
        if _arg == f'@{BotName}':
            return trival
        if _arg == f'@{BotName}@www.loganjohndarylgraham.xyz':
            return trival
    return -1


async def cmd(author,message,instance,bot_ref):
    reply='NULL'
    args = str(message).split(' ')
    trival= await BotMentioned(message,'MeSoBot')
    mentions=''

    for msgz in args:
        if msgz[0] == '@':
            mentions=f'{mentions} {msgz}'

    if trival > 0: #only execute if the bot is tagged at the start of the post 
        shellargs=str('')
        for sarg in args[trival:]:
            shellargs=shellargs+sarg+str(' ')
        print(f'shell {shellargs}')
        reply = await shell(shellargs.split(' '),f'{author}@{instance}',trival)
        string_log=str(f'[{message}] -> @{author}_{instance}')
        if reply != 'NULL':
            await Log(string_log)
            reply=f'{mentions}\n{reply}'
    return reply

async def Messaged(bot_ref,note: Note):
    instance_name = note.author.instance.name if note.author.instance else 'local'
    username = note.author.nickname or note.author.name
    msga=str(note.content).replace("\n","")
    trival= await BotMentioned(msga,'MeSoBot')
    if note.author.name != "MeSoBot":
        if trival >= 1:
            message_reply = await cmd(note.author.name,msga,instance_name,bot_ref)
            if message_reply != 'NULL':
                await note.reply('%s' % message_reply)
            else:
                await note.reply('I Do not know what this means. Try \'help\',\n later I might know how to converse but not yet.')
