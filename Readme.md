#running the bot
create a `MeSoBot_Data.json` file.

```JSON
{
    "uri":"wss://[INSERT WEBSITE]/streaming",
    "token":"[INSERT TOKEN]",
    "AllowedShellCommands":[
        "help",
        "date",
        "calculate",
        "info",
        "fizz_buzz",
        "set",
        "get",
        "delete",
        "list"
    ],
    "ArgumentCountShellCommands":[
        0,
        1,
        0,
        0,
        2,
        1,
        1,
        0
    ],
    "Channels":["global"]
}
```

- AllowedShellCommands is a list of commands allowed to be executed. the commands are shell scripts in the directory `shelldir/`

- The main 'executable' is `app.py`

- you will need to have install Mi.py from
`https://github.com/yupix/Mi.py`

# other notes
- if you edit `Brain.py` take notice that no variables will stick permanently (at least to my knowledge)
because i reload this module/file ever time we recieve an event/message/post.
this is so we can edit this `Brain.py` that controls the bot without having to restart the bot.

- note that you will need to of course modify the shelldir/* files to match your systems equivalent or at least have the called programs installed.
and for the shellLib/* i use `cut`,`tr`,`wc` and `calc`.
suggestions for alternative ways of doing the same thing are welcome
but i would prefer to keep these files as shell scripts as much as possible.