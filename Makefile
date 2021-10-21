server_dir = ./server_src
client_dir = ./client_src
utils_dir = ./utils_src

obj_dir = objs

#src = ${wildcard *.cpp}
#src_server = ${wildcard ${server_dir}/*.cpp}
#src_client = ${wildcard ${client_dir}/*.cpp}
src_multi_server = ${server_dir}/multithreading_echo_server.cpp ${utils_dir}/net_write_read.cpp
src_client_peps = ${client_dir}/peps_client.cpp ${utils_dir}/net_write_read.cpp
src_client_multi_echo = ${client_dir}/multithreading_echo_client.cpp ${utils_dir}/net_write_read.cpp

header = ${wildcard *.hpp}

#obj = ${src:%.cpp=%.o}
obj_server = ${src_server: ${server_dir}/%.cpp=${obj_dir}/%.o}
obj_client = ${src_client: ${client_dir}/%.cpp=${obj_dir}/%.o}
obj_multi_server = ${src_multi_server:%.cpp=${obj_dir}/%.o}
obj_client_peps = ${src_client_peps:%.cpp=${obj_dir}/%.o}
obj_client_multi_echo = ${src_client_multi_echo:%.cpp=${obj_dir}/%.o}

cc = clang++

flags = -Wall -Wextra -Werror -std=c++98 # -Wshadow -Wno-shadow

name_client = client
name_server = server
name_server_multi = server_multi
name_client_peps = peps_client
name_client_multi_echo = multithreading_echo_client

rm = rm -rf

${name_client}:	${obj_client} ${header} Makefile
	${cc} ${flags} ${obj_client} -o ${name_client}

${name_server}:	${obj_server} ${header} Makefile
	${cc} ${flags} ${obj_server} -o ${name_server}

${name_server_multi}: ${obj_multi_server} ${header}
	${cc} ${flags} $^ -o $@

${name_client_peps}:  ${obj_client_peps} ${header} Makefile
	${cc} ${flags} ${obj_client_peps} -o ${name_client_peps}

${name_client_multi_echo}:   ${obj_client_multi_echo} ${header} Makefile
	${cc} ${flags} ${obj_client_multi_echo} -o ${name_client_multi_echo}

$(obj_dir)/%.o: %.cpp	Makefile
	mkdir -p $(dir $@)
	$(cc) $(flags) -c $< -o $@
	@# echo "Compiled "$<" successfully!"
#$(objdir)/%.o	: $(srcdir)/%.c Makefile $(inclds)
#				@mkdir -p $(dir $@)
#				$(cc) $(cflags) -c $< -o $@
#				@echo "Compiled "$<" successfully!"

all:

clean:
	${rm} ${obj_dir}

fclean: clean
	${rm} ${name_client} ${name_server} ${name_server_multi} ${name_client_peps} ${name_client_multi_echo}

re: fclean all

.PHONY: clean fclean all
