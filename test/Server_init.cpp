#include "Server.hpp"

void	signalHandler(int signum) {
	std::cout << "Interrupt signal (" << signum << ") received." << std::endl;
	server_life = 0;
	close(sock_fd);
	exit(1);
}

static void	ft_init_error(std::string const &str){
	perror(str.c_str());
	server_life = 0;
	close(sock_fd);
	exit(1);
}

static int	get_socket() {
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0)
		ft_init_error("Socker error");
	fcntl(fd, F_SETFL, O_NONBLOCK); // ???
	sock_fd = fd;
	return (fd);
}

static void bind_and_listen_socket(t_server &srv) {
	int	result;
	
	result = bind(srv.socket, (sockaddr *)&srv.addr, sizeof(srv.addr));
	if (result)
		perror("Bind socket error");
	std::cout << "Socket bound to address" << std::endl;

	result = listen(srv.socket, 128);
	if (result)
		perror("Listen error");
	std::cout << "Socket ready to listen for connections" << std::endl;	
}

static void ft_epoll_add_srv(t_server &srv) {
	int res_one, res_two;
	
	srv.ev_def.data.fd = STDIN_FILENO;
	res_one = epoll_ctl(srv.poll_fd, EPOLL_CTL_ADD, STDIN_FILENO, &srv.ev_def);
	
	srv.ev_def.data.fd = srv.socket;
	res_two = epoll_ctl(srv.poll_fd, EPOLL_CTL_ADD, sock_fd, &srv.ev_def);
	
	if (res_one || res_two) {
		close(srv.poll_fd);
		perror("epoll_ctl() failer");
	}
}

void	init_server(std::string const &port, std::string const &passwd, t_server &srv) {

	srv.port = atoi(port.c_str()); // use strdol??
	srv.passwd = passwd;
	srv.socket = get_socket(); std::cout << "Socket created" << std::endl;
	srv.addr.sin_family = AF_INET; // IPv4
	srv.addr.sin_addr.s_addr = INADDR_ANY; // Any IP address
	srv.addr.sin_port = htons(srv.port);
	srv.client_addr_len = sizeof(srv.client_addr);
	
	bind_and_listen_socket(srv);
	signal(SIGINT, signalHandler);

	srv.poll_fd = epoll_create1(0);
	srv.ev_def.events = EPOLLIN;

	ft_epoll_add_srv(srv);
}
