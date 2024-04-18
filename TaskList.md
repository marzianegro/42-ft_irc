# To-Do's for `ft_irc`

## Things to fix
- [ ] +o works (supposedly), but -o doesn't						->
- [x] User can JOIN the same channel multiple times				-> added check for user already being on channel
- [x] QUIT is not implemented yet OR it doesn't work???			-> changed final three statements order
- [x] leave channel												-> implemented PART command but @Gigi needs to implement parseQuit in parseCmds.cpp
- [x] make sure leave channel does _userCount--					-> also removed user from channel's list of user (was missing)
- [x] check if you can set limit lower than user count			-> added check through if/else
- [ ] do not permit to set limit over 2142						->
- [ ] rifare tutti gli errori fanno cacare su questo client		->
- [x] port check doesn't work with letters						-> added check in checkPort, now passing it av[i] instead of atoi(av[1])
- [x] fix & when joining a channel								 -> straight up guessed lol (added check for '&' in check for '#')
- [ ] check conflicts between I mode and X mode					 ->

## Things to check
- [x] mode +i
- [x] mode -i
- [ ] mode +o
- [ ] mode -o
- [x] mode +k
- [x] mode -k
- [x] mode +t
- [x] mode -t
- [x] mode +l
- [x] mode -l
- [ ] Server::setTopic
- [ ] Server::getTopic
- [ ] WHO command (volendo si può fare che non fa una cippa, oppure si implementa visto che e' gia' presente in join)
  
