#include "get_next_line.h"

// Function to free the memory allocated for a string pointer
char	*ft_free(char **str)
{
	free(*str);
	*str = NULL;
	return (NULL);
}

// Function to clean the storage and return the remaining part after a newline character
char	*clean_storage(char *storage)
{
	char	*new_storage;
	char	*ptr;
	int		len;

	ptr = ft_strchr(storage, '\n');
	if (!ptr)
	{
		new_storage = NULL;
		return (ft_free(&storage));
	}
	else
		len = (ptr - storage) + 1;
	if (!storage[len])
		return (ft_free(&storage));
	new_storage = ft_substr(storage, len, ft_strlen(storage) - len);
	ft_free(&storage);
	if (!new_storage)
		return (NULL);
	return (new_storage);
}

// Function to extract the line from the storage until the newline character
char	*new_line(char *storage)
{
	char	*line;
	char	*ptr;
	int		len;

	ptr = ft_strchr(storage, '\n');
	len = (ptr - storage) + 1;
	line = ft_substr(storage, 0, len);
	if (!line)
		return (NULL);
	return (line);
}

// Function to read from the file descriptor into the storage buffer
char	*readbuf(int fd, char *storage)
{
	int		rid;      // Variable to store the result of the read() function
	char	*buffer;  // Buffer to read data from the file descriptor

	rid = 1;
	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));  // Allocate memory for the buffer
	if (!buffer)
		return (ft_free(&storage));  // Return NULL if memory allocation fails
	buffer[0] = '\0';

	while (rid > 0 && !ft_strchr(buffer, '\n'))
	{
		rid = read(fd, buffer, BUFFER_SIZE);  // Read data from the file descriptor into the buffer
		if (rid > 0)
		{
			buffer[rid] = '\0';  // Null-terminate the buffer after reading data
			storage = ft_strjoin(storage, buffer);  // Append the buffer to the storage
		}
	}

	free(buffer);  // Free the memory allocated for the buffer

	if (rid == -1)
		return (ft_free(&storage));  // Return NULL if there was an error in reading from the file descriptor

	return (storage);  // Return the updated storage containing the read data
}


// Main function to get the next line from the file descriptor
char	*get_next_line(int fd)
{
	static char	*storage = {0};
	char		*line;

	if (fd < 0)
		return (NULL);

	/*
	Checks if the storage buffer contains
    a new line (\n) or if it is empty. If the buffer does not contain
    a new line or is empty, the readbuf function is called for
    read more data from the file descriptor and update the buffer
    of storage.
	*/
	if ((storage && !ft_strchr(storage, '\n')) || !storage)
		storage = readbuf(fd, storage);

	if (!storage)
		return (NULL);
	line = new_line(storage);
	if (!line)
		return (ft_free(&storage));
	storage = clean_storage(storage);
	return (line);
}

/*
use this command to compile the program
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c -o get_next_line
*/
int main(void)
{
    int fd = open("test.txt", O_RDONLY); // Replace "test.txt" with the actual file name you want to read from
    char *line;

    // Read lines from the file until the end is reached
    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line); // Print each line
        free(line); // Free the memory allocated for the line
    }

    close(fd); // Close the file descriptor

    return 0;
}
