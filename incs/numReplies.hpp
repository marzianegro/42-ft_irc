#pragma once

#include "Channel.hpp"
#include "Server.hpp"

std::string errNoSuchNick(const std::string &sender, const std::string &receiver);
std::string errNoSuchChannel(const std::string &channelName, const std::string &clientName);
std::string errNoOrigin(const std::string &clientName);
std::string errNoTextToSend(const std::string &sender);
std::string errUnknownCommand(const std::string &clientName, const std::string &command);
std::string errNoNicknameGiven(const std::string &clientName);
std::string errErroneusNickname(const std::string &clientName, const std::string &nickname);
std::string errNicknameInUse(const std::string &clientName, const std::string &nickname);
std::string errNotOnChannel(const std::string &channelName, const std::string &clientName);
std::string errUserNotInChannel(const std::string &channelName, const std::string &clientName, const std::string &invitedClientName);
std::string errUserOnChannel(const std::string &channelName, const std::string &clientName, const std::string &invitedClientName);
std::string errNotRegistered();
std::string errAlreadyRegistered(const std::string &clientName);
std::string errPasswdMismatch(const std::string &clientName);
std::string errNeedMoreParams(const std::string &clientName, const std::string &command);
std::string errChannelIsFull(const std::string &channelName, const std::string &clientName);
std::string errInviteOnlyChan(const std::string &channelName, const std::string &clientName);
std::string errBadChannelKey(const std::string &channelName, const std::string &clientName);
std::string errChanOPrivsNeeded(const std::string &channelName, const std::string &clientName);

std::string rplWelcome(const std::string &clientName);
std::string rplChannelModeIs(const std::string &channelName, const std::string &clientName, Channel *channel);
std::string rplNoTopic(const std::string &channelName, const std::string &clientName);
std::string rplTopic(const std::string &channelName, const std::string &clientName, const std::string &topic);
std::string rplInviting(const std::string &channelName, const std::string &clientName, const std::string &invitedClientName);
std::string rplNamReply(const Channel &channel, const std::string &clientName);
std::string rplEndOfNames(const std::string &channelName, const std::string &clientName);
