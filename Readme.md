#running the bot
create a `MeSoBot_Data.json` file.

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

- The main 'executable' is `app.py`

- you will need to have install Mi.py from
`https://github.com/yupix/Mi.py`