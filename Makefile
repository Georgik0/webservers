name = webserv
objdir = objs
cgidir = cgi

sources = Socket.cpp main.cpp Select.cpp \
		  parse_header.cpp \
		  parse_config.cpp Server.cpp answer.cpp \
		  utils.cpp autoindex.cpp

objects = $(sources:%.cpp=$(objdir)/%.o)
deps	= $(objects:%.o=%.d)

linker = clang++
flags = -g -std=c++11 #-fsanitize=address #-Wall -Wextra -Werror 
rm	  = rm -rf

all: $(name)

cgi:
	$(MAKE) -C $(cgidir)/

$(name): $(objects) cgi
	$(linker) $(flags) $(objects) -o $@
	$(MAKE) -C $(cgidir)/

-include $(deps)
$(objdir)/%.o:	%.cpp Makefile
	@mkdir -p $(objdir)
	$(linker) $(flags) -MMD -c $< -o $@

clean:
	$(rm) $(objdir)

fclean: clean
	$(rm) $(name)

re: fclean all

leaks:
	leaks --atExit -- ./$(name)
