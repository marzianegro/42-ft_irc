# To-Do's for `ft_irc`

## Things to fix
- [x] check conflicts between I mode and K mode [dovrei aver gestito questa cosa ma e' da testare]
		if I -> K is disabled
		cant set K if I is set

		if I is set, when i try to set K, it keeps saying mode I, not sure this is how it's supposed to work
- [?] print mode non printa bene
- [x] PART shows that user left channel for other people but not for who's leaving + channel remains visible
- [x] `/mode +i` prints on channel, doesn't look that good, so consider adding numRpl for mode setting and unsetting [ora dovrebbe essere carino]
		* mnegro sets mode +i on #test
		* Channel #test modes: i
- [?] setting mode L through button on hexchat does not work [ricontrolla per favore, non trovo il bottone su hexchat]
- [x] if K mode is set, i can still set mode I (they are in a queue, if i unset K, then I is valid) [testalo ma dovrebbe essere okay]
- [?] PART does not work if channel name does not have # [in che senso? serve una replica]
- [x] you can do `/mode -o` on yourself, is it correct? [tutto okay, giusto cosi, se vuoi puoi mettere che se la lista di op e' == 1 allora non puoi fare deop]
- [x] `mode +l 1` does not work BUT i think only if the user count > user limit BUT this does not print anything, maybe it should for clarity's sake [should be okay, but check it please]
- [x] if the channel operator leaves and there are no more operators, there's no operator left [dovrebbe essere okay, ho aggiunto la funzione server->checkChOperators(channel)]
- [ ] check cmds.cpp:150
- [ ] memory leaks post modifiche eheheh ma non ci dovrebbero essere


## Side quests
- [ ] rifare tutti gli errori fanno cacare su questo client [boh non sono in realta' cosi sbaglati]
- [ ] if `RPL_TOPIC` is returned to the client sending this command, `RPL_TOPICWHOTIME` SHOULD also be sent to that client.

## Things to check
- [ ] mode +i
- [ ] mode -i
- [ ] mode +o
- [ ] mode -o
- [ ] mode +k
- [ ] mode -k
- [ ] mode +t
- [ ] mode -t
- [ ] mode +l
- [ ] mode -l
- [ ] Server::setTopic
- [ ] Server::getTopic
- [ ] WHO command (only for channel, to be tested)

## Done
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
- [x] run `valgrind --leak-check=full --show-leak-kinds=all`
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

## FixMe
- [ ] remove additional comment for setting mode +k (already handled by client)
- [ ] setting channel limit through button also prints active modes in channel
- [ ] add "Error: " to "User limit must be between 2 and 2142"
- [ ] add a message that you can't deop yourself if you're the only operator in channel
