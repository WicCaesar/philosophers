MAKEFLAGS = --silent
#BLACK			= 	\033[1;30m			#RED			=	\033[1;31m
GREEN			=	\033[3;32m
#YELLOW      	=	\033[1;33m			#BLUE			=	\033[1;34m
#MAGENTA		=	\033[1;35m			#CYAN			=	\033[1;36m
#WHITE			=	\033[4;37m
DEFAULT			=	\033[0m
#ONELINEUP		=	"\033[1A"			#ERASELINE		=	"\033[K"
#NORMAL			[0;		#BOLD			[1;		#DIM			[2;
#ITALIC			[3;		#UNDERLINED		[4;		#BLINK			[5;
#REVERSEBG		[7;		#HIDDEN			[8;		#STRIKETHROUGH	[9;
# To format, use the following syntax: \033[STYLE;TEXT;BACKGROUNDm
# For example, to print a bold red text on a blue background, use: \033[1;31;44m
# To combine styles, use a semicolon: \033[1;4;31m (bold + underlined, red text)
# Rainbow: \033[1;31mR\033[1;33mY\033[1;32mG\033[1;36mC\033[1;34mB\033[1;35mM
NAME	= philo
SOURCESPATH = ./
SOURCESLIST = sophos.c arithmos.c plithysmos.c kyklos.c makaronia.c hronos.c
SOURCES = $(addprefix $(SOURCESPATH), $(SOURCESLIST))
OBJECTS = $(SOURCES:.c=.o)

#ifeq ($(shell uname), Darwin)	#MacOS
#	SYSTEM		= @printf "\e[1;33mMacOS\n\e[m"
#	MINILIBX	= libmlx
#	MLXFLAGS 	= -lmlx -framework OpenGL -framework AppKit -L ./$(MINILIBX)
#else 	#Linux
#	SYSTEM		= @printf "\e[1;33mLinux\n\e[m"
#	MINILIBX	= libmlx_Linux
#	MLXFLAGS	= -lmlx -lXext -lX11 -L ./$(MINILIBX)
#endif

COMPILE	= cc
FLAGS	= -Wall -Wextra -Werror -g
REMOVE	= rm -f

# In order to pass an argument to the compiler, use the -D flag and set a
# variable such as $(X) and when calling make, add X = 3, for instance.

# Compiles from open-source to binary, but doesn't link. Necessary to run flags.
.c.o:
	$(COMPILE) $(FLAGS) -c $< -o $(<:.c=.o)

# Calling its name acts as its own makefile. Will compile only if the object 
# dependencies are fulfilled. ar creates an archive (here, library) from the 
# files member (objects), replacing them as needed. ranlib simply indexes 
# every function in the library (equivalent to s in crs).
# It searches for the libft library in its original location, avoiding relinks.
all:	$(NAME)

$(NAME):	$(OBJECTS)
	$(COMPILE) $(FLAGS) $(OBJECTS) -lpthread -o $(NAME)
#	echo "$(GREEN)Pronto!$(DEFAULT) Execute $(WHITE)./philo convidados inanição\
#	jantar sesta repeteco$(DEFAULT)"
#	echo "\033[3mconvidados = número de filósofos à mesa"
#	echo " inanição  = tempo em milissegundos que aguentam sem comer"
#	echo "  jantar   = tempo em milissegundos que demoram comendo o espaguete"
#	echo "   sesta   = tempo em milissegundos que levam descansando"
#	echo " repeteco  = quantidade de pratos que convidados precisam repetir"
#	echo "\033[4mApenas valores numéricos.$(DEFAULT)"
#* -lpthread links the pthread library, which is necessary for multithreading.

# '@' hides the clutter from the terminal.
clean:
	@$(REMOVE) $(OBJECTS) $(OBJECTSBONUS)
	@echo "$(GREEN)Limpíssimo!$(DEFAULT)"

fclean:		clean
	@$(REMOVE) $(NAME) $(NAMEBONUS)

re:		fclean all

#NAMEBONUS	=	philo_bonus
#PATHBONUS	=	./philo_bonus
#LISTBONUS	=	sophos_bonus.c
#SOURCESBONUS	=	$(addprefix $(PATHBONUS), $(LISTBONUS))
#OBJECTSBONUS	=	$(SOURCESBONUS:.c=.o)

#bonus:	$(NAMEBONUS)

#$(NAMEBONUS):	$(OBJECTSBONUS)
#	$(COMPILE) $(FLAGS) $(OBJECTSBONUS) -o $(NAMEBONUS)

run:	$(NAME)
	./$(NAME) 2 800 200 200

vrun:	$(NAME)
	valgrind --leak-check=full -s -v ./$(NAME) 400 200 200

rerun:	re run

# Ignores files that could eventually be named after any of these rules.
.PHONY:		all clean fclean re