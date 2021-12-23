name = cgi
objdir = objs

sources = cgi.cpp generate_html.cpp

objects = $(sources:%.cpp=$(objdir)/%.o)
deps	= $(objects:%.o=%.d)

linker = clang++
flags = -g -std=c++98 #-Wall -Wextra -Werror #-fsanitize=address
rm	  = rm -rf

all: $(name)

$(name): $(objects)
	$(linker) $(flags) $^ -o $@

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
