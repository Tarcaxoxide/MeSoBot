
github:
	git add .
	git commit -am "$(shell date)"
	git push "https://loganer%40vivaldi.net:$(shell cat ../git_token.txt)@github.com/Tarcaxoxide/MeSoBot.git"