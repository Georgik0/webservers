server_dir = ./server_src
client_dir = ./client_src
utils_dir = ./utils_src

obj_dir = objs

#src = ${wildcard *.cpp}
#src_server = ${wildcard ${server_dir}/*.cpp}
#src_client = ${wildcard ${client_dir}/*.cpp}
src_multi_server = ${server_dir}/multithreading_echo_server.cpp ${utils_dir}/net_write_read.cpp
src_server_select = ${server_dir}/select_server.cpp ${utils_dir}/net_write_read.cpp
src_server_multi_select = ${server_dir}/multithreading_select_server.cpp ${server_dir}/workers.cpp ${utils_dir}/net_write_read.cpp
src_client_peps = ${client_dir}/peps_client.cpp ${utils_dir}/net_write_read.cpp
src_client_multi_echo = ${client_dir}/multithreading_echo_client.cpp ${utils_dir}/net_write_read.cpp

header = ${wildcard *.hpp} ${wildcard ${server_dir}/*.hpp}

#obj = ${src:%.cpp=%.o}
obj_server = ${src_server: ${server_dir}/%.cpp=${obj_dir}/%.o}
obj_server_select = ${src_server_select:/%.cpp=${obj_dir}/%.o}
obj_client = ${src_client: ${client_dir}/%.cpp=${obj_dir}/%.o}
obj_multi_server = ${src_multi_server:%.cpp=${obj_dir}/%.o}
obj_multi_select_server = ${src_server_multi_select:%.cpp=${obj_dir}/%.o}
obj_client_peps = ${src_client_peps:%.cpp=${obj_dir}/%.o}
obj_client_multi_echo = ${src_client_multi_echo:%.cpp=${obj_dir}/%.o}

cc = clang++

flags = -Wall -Wextra -Werror -std=c++11 -fsanitize=address # -Wshadow -Wno-shadow

name_client = client
name_server = server
name_server_multi = server_multithreading_echo
name_server_select = server_select
name_server_multi_select = server_multithreading_select
name_client_peps = client_peps
name_client_multi_echo = client_multithreading_echo

rm = rm -rf

${name_server_multi}: ${obj_multi_server}
	${cc} ${flags} $^ -o ${name_server}

${name_client_peps}:  ${obj_client_peps}
	${cc} ${flags} $^ -o ${name_client}

${name_client_multi_echo}:   ${obj_client_multi_echo}
	${cc} ${flags} $^ -o ${name_client}

${name_server_select}:	${obj_server_select}
	${cc} ${flags} $^ -o ${name_server}

${name_server_multi_select}: ${obj_multi_select_server}
	${cc} ${flags} $^ -o ${name_server}

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
	${rm} ${name_client} ${name_server} ${name_server_multi} ${name_client_peps} ${name_client_multi_echo} client

re: fclean all

.PHONY: clean fclean all
