#pragma once

#include "Channel.hpp"
#include "Server.hpp"

std::string errNoSuchChannel(const std::string &channelName, const std::string &clientName);
std::string errNotOnChannel(const std::string &channelName, const std::string &clientName);
std::string errUserNotInChannel(const std::string &channelName, const std::string &clientName, const std::string &invitedClientName);
std::string errUserOnChannel(const std::string &channelName, const std::string &clientName, const std::string &invitedClientName);
std::string errNeedMoreParams(const std::string &clientName, const std::string &command);
std::string errChannelIsFull(const std::string &channelName, const std::string &clientName);
std::string errInviteOnlyChan(const std::string &channelName, const std::string &clientName);
std::string errBadChannelKey(const std::string &channelName, const std::string &clientName);
std::string errChanOPrivsNeeded(const std::string &channelName, const std::string &clientName);

std::string rplNoTopic(const std::string &channelName, const std::string &clientName);
std::string rplTopic(const std::string &channelName, const std::string &clientName, const std::string &topic);
std::string rplInviting(const std::string &channelName, const std::string &clientName, const std::string &invitedClientName);
std::string rplNamReply(const Channel &channel, const std::string &clientName);
std::string rplEndOfNames(const std::string &channelName, const std::string &clientName);
