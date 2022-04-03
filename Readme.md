#running the bot
create a json file 1 directory above this one.

```JSON
{
    "uri":"wss://[INSERT WEBSITE]/streaming",
    "token":"[INSERT TOKEN]",
    "AllowedShellCommands":[
        "date",
        "calculate"
    ],
    "Channels":["local","global","main"]
}
```

- AllowedShellCommands is a list of commands allowed to be executed. the commands are shell scripts in the directory `shelldir/`