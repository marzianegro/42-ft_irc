# To-Do's for `ft_irc`

## Things to fix
- [x] +o works (supposedly), but -o doesn't						-> while (it_user++ ...) messed it up, so moved ++it_user to the end of the loop
- [x] User can JOIN the same channel multiple times				-> added check for user already being on channel
- [x] QUIT is not implemented yet OR it doesn't work???			-> changed final three statements order
- [x] leave channel												-> implemented PART command + @Gigi implemented parsePart in parseCmds.cpp
- [x] make sure leave channel does _userCount--					-> also removed user from channel's list of user (was missing)
- [x] check if you can set limit lower than user count			-> added check through if/else
- [ ] rifare tutti gli errori fanno cacare su questo client		-> // FIXME: @Gigi
- [x] port check doesn't work with letters						-> added check in checkPort, now passing it av[i] instead of atoi(av[1])
- [x] fix & when joining a channel								 -> straight up guessed lol (added check for '&' in check for '#')
- [ ] check conflicts between I mode and K mode
		if I -> K is disabled
		cant set K if I is set

		if I is set, when i try to set K, it keeps saying mode I, not sure this is how it's supposed to work

- [x] add WHO command
- [x] make & possible in channel name							-> to be tested, but was already setup for it
- [x] check how clients are cancelled (`nc` still receives message from channel after killing it)
- [X] when user exits, make sure everythig is clean
	```
	in executiÍon: JOIN #culo  
	CALLING JOIN: #culo  
	JOINING: #culo WITH KEY:   
	User gigetto found in _opUsers  
	terminate called after throwing an instance of 'std::logic_error'  
	what():  basic_string: construction from null is not valid  
	[1]    4921 IOT instruction (core dumped)  ./ircserv 8080 topolo  
	```
- [x] `kick` kicks but not in client
- [x] run `valgrind --leak-check=full --show-leak-kinds=all`
- [x] implement ctrl-C
- [x] implement notice for `part` -> found out it's implemented but doesn't work
- [x] implement notice for `nick`
- [x] implement notice for `user` -> If a client tries to send the USER command after they have already completed registration with the server, the ERR_ALREADYREGISTERED reply should be sent and the attempt should fail
- [ ] print mode non printa bene
- [ ] PART shows that user left channel for other people but not for who's leaving + channel remains visible
- [ ] `/mode +i` prints on channel, doesn't look that good, so consider adding numRpl for mode setting and unsetting
		* mnegro sets mode +i on #test
		* Channel #test modes: i
- [ ] setting mode L through button on hexchat does not work
- [ ] if K mode is set, i can still set mode I (they are in a queue, if i unset K, then I is valid)
- [ ] PART does not work if channel name does not have #
- [ ] you can do `/mode -o` on yourself, is it correct?
- [ ] `mode +l 1` does not work BUT i think only if the user count > user limit BUT this does not print anything, maybe it should for clarity's sake


## Side quests
- [ ] if `RPL_TOPIC` is returned to the client sending this command, `RPL_TOPICWHOTIME` SHOULD also be sent to that client.

## Things to check
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
- [ ] Server::setTopic
- [ ] Server::getTopic
- [ ] WHO command (only for channel, to be tested)

## Done
- [x] do not permit to set limit over 2142
