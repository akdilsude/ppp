SRCS	=	cleanup.c		\
			check.c			\
			monitor.c		\
			main.c			\
			setup.c			\
			routine.c		\
			report.c		\
			time.c			\

OBJS	=	$(SRCS:.c=.o)
RM		=	rm -f
CFLAGS	=	-Wall -Wextra -Werror
CC 		= 	cc
NAME	=	philo

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -pthread

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re