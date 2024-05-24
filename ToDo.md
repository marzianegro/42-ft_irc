# To-Do's for `ft_irc`

## Fix 🛠️
- [x] "Channel #name modes: " printa male, non si capisce niente -> automatico???
- [ ] setting channel limit through button also prints active modes in channel -> check
- [ ] run `valgrind --leak-check=full --show-leak-kinds=all`
- [ ] test /INVITE with wrong username and also with no channel name

## Check 🔍
- [x] mode +i
- [x] mode -i
- [x] mode +o
- [x] mode -o
- [x] mode +k
- [x] mode -k
- [x] mode +t
- [x] mode -t
- [x] mode +l
- [x] mode -l
- [x] Server::setTopic
	TOPIC #test :New topic          ; Setting the topic on "#test" to
									"New topic".
	TOPIC #test :                   ; Clearing the topic on "#test"	
	TOPIC #test                     ; Checking the topic for "#test"
- [x] Server::getTopic
- [x] WHO command (only for channel, to be tested) -> this is what it prints (command has to be precisely
	/WHO #test)
	WHO #test
	:gerboa 353 fakegigi = #test :@debtopo  fakegigi 
	:gerboa 366 fakegigi #test :End of /NAMES list
	:debtopo PRIVMSG #test :WHO
	:debtopo PRIVMSG #test :WHO #test

## Side quests 🚥
- [ ] rifare tutti gli errori fanno cacare su questo client [boh non sono in realta' cosi sbaglati]
- [ ] if `RPL_TOPIC` is returned to the client sending this command, `RPL_TOPICWHOTIME` SHOULD also be sent to that client.

## Done 👏
- [x] port check doesn't work with letters						-> added check in checkPort, now passing it av[i] instead of atoi(av[1])
- [x] fix & when joining a channel								 -> straight up guessed lol (added check for '&' in check for '#')
- [x] do not permit to set limit over 2142
- [x] +o works (supposedly), but -o doesn't						-> while (it_user++ ...) messed it up, so moved ++it_user to the end of the loop
- [x] User can JOIN the same channel multiple times				-> added check for user already being on channel
- [x] QUIT is not implemented yet OR it doesn't work???			-> changed final three statements order
- [x] leave channel												-> implemented PART command + @Gigi implemented parsePart in parseCmds.cpp
- [x] make sure leave channel does _userCount--					-> also removed user from channel's list of user (was missing)
- [x] check if you can set limit lower than user count			-> added check through if/else
- [x] add WHO command
- [x] make & possible in channel name							-> to be tested, but was already setup for it
- [x] check how clients are cancelled (`nc` still receives message from channel after killing it)
- [X] when user exits, make sure everythig is clean
- [x] `kick` kicks but not in client
- [x] implement ctrl-C
- [x] implement notice for `part` -> found out it's implemented but doesn't work
- [x] implement notice for `nick`
- [x] implement notice for `user` -> If a client tries to send the USER command after they have already completed registration with the server, the ERR_ALREADYREGISTERED reply should be sent and the attempt should fail

	```
	in executiÍon: JOIN #culo  
	CALLING JOIN: #culo  
	JOINING: #culo WITH KEY:   
	User gigetto found in _opUsers  
	terminate called after throwing an instance of 'std::logic_error'  
	what():  basic_string: construction from null is not valid  
	[1]    4921 IOT instruction (core dumped)  ./ircserv 8080 topolo  
	```
- [x] remove additional comment for setting mode +k (already handled by client)
- [x] add "Error: " to "User limit must be between 2 and 2142"
- [x] add a message that you can't deop yourself if you're the only operator in channel
- [x] check conflicts between I mode and K mode [dovrei aver gestito questa cosa ma e' da testare]
		if I -> K is disabled
		cant set K if I is set

		if I is set, when i try to set K, it keeps saying mode I, not sure this is how it's supposed to work
- [x] PART shows that user left channel for other people but not for who's leaving + channel remains visible
- [x] `/mode +i` prints on channel, doesn't look that good, so consider adding numRpl for mode setting and unsetting [ora dovrebbe essere carino]
		* mnegro sets mode +i on #test
		* Channel #test modes: i
- [x] if K mode is set, i can still set mode I (they are in a queue, if i unset K, then I is valid) [testalo ma dovrebbe essere okay]
- [x] `mode +l 1` does not work BUT i think only if the user count > user limit BUT this does not print anything, maybe it should for clarity's sake [should be okay, but check it please]
- [x] if the channel operator leaves and there are no more operators, there's no operator left [dovrebbe essere okay, ho aggiunto la funzione server->checkChOperators(channel)]
- [x] check cmds.cpp:150
- [x] you can do `/mode -o` on yourself, is it correct? [tutto okay, giusto cosi, se vuoi puoi mettere che se la lista di op e' == 1 allora non puoi fare deop]
- [x] setting mode L through button on hexchat does not work -> jk, it works
- [x] add NOTICE for +k without a key
- [x] add NOTICE for +k when +i is already enabled
- [x] add NOTICE for trying to disable modes when they're not enabled
- [x] no issues if -i even when +i is not set, is it correct?
- [x] missing space here: mnegrois already a channel operator
- [x] ho fatto /part da #chan, poi ho joinato di nuovo #chan dalla schermata principale,
		poi ho fatto /part di nuovo e /join di nuovo ma questa volta dalla schermata del canale

		in execution: JOIN chan
		CALLING JOIN: chan
		JOINING: #chan WITH KEY: 
		User mnegro found in _regUsers
		terminate called after throwing an instance of 'std::logic_error'
		what():  basic_string::_M_construct null not valid
		[1]    69739 abort (core dumped)  ./ircserv 8080 topolo
- [x] go over all highlighted comments and fix them
- [x] no issues if -k has a key specified, is it correct?
