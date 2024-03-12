#include "Server.hpp"
#include <sstream>

int	server_exit(t_server &srv, int exit_code) {
	close(srv.client_fd);
	close(srv.socket);
	close(srv.poll_fd);
	/* loop for al list */

	exit(exit_code);
}

void	wait_events(t_server &srv, int &ev_nums) {
	ev_nums = epoll_wait(srv.poll_fd, srv.ev_lst, MAX_EVENTS, -1);
	if (ev_nums == -1) {
		perror("Error in epoll()");
		server_exit(srv, 1);
	}
	// std::cout << "num events: " << ev_nums << std::endl;
}

void console_event(t_server &srv) {
	std::cin >> srv.buffer;
	if (!strcmp(srv.buffer, "quit") || !strcmp(srv.buffer, "exit"))
		server_exit(srv, 1);
}

void new_client_event(t_server &srv) {
	int result;
	
	srv.client_fd = accept(srv.socket, (sockaddr *)&srv.client_addr, &srv.client_addr_len);
	if (srv.client_fd < 0) {
		std::cerr << "Failed to accept the client" << std::endl;
		close(srv.socket);
		return ;
	}
	srv.ev_def.events = EPOLLIN;
	srv.ev_def.data.fd = srv.client_fd;
	
	result = epoll_ctl(srv.poll_fd, EPOLL_CTL_ADD, srv.client_fd, &srv.ev_def);
	if (result) {
		perror("Error adding client to epoll");
		return ;
	}
}

void	new_cmd_event(t_server &srv, int i) {
	srv.client_fd = srv.ev_lst[i].data.fd;
	srv.bytes_read = recv(srv.client_fd, srv.buffer, sizeof(srv.buffer), 0);

	if (!srv.bytes_read) {
		std::cerr << "Connection closed by the client" << std::endl;
		epoll_ctl(srv.poll_fd, EPOLL_CTL_DEL, srv.client_fd, &srv.ev_lst[i]);
	}
	else if (srv.bytes_read < 0)
		std::cerr << "Error while receving the message" << std::endl;
	else
	{
		srv.buffer[srv.bytes_read] = '\0';
		std::cout << "msg: " << srv.buffer; 
	}
}

void	ft_server_life(t_server &srv) {
	int	ev_nums;
	
	while (server_life)
	{
		wait_events(srv, ev_nums);

		std::cout << "ev_nums: " << ev_nums << std::endl; 
		for (int i = 0; i < ev_nums; i++)
		{
			if (srv.ev_lst[i].data.fd == STDIN_FILENO)
				console_event(srv);
			else if (srv.ev_lst[i].data.fd == sock_fd)
				new_client_event(srv);
			else
				new_cmd_event(srv, i);
		}
	}
}

