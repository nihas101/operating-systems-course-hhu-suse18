#include <unistd.h>
#include <sys/types.h>
#include <limits.h>

int main(){
	uid_t user_id;

	int hostname_ret;
	char *hostname = malloc(HOST_NAME_MAX*sizeof(char));

	char *current_working_dir;

	// USER ID
	user_id = getuid();
	printf("UID: %d\n", user_id);

	// HOSTNAME
	hostname_ret = gethostname(hostname, HOST_NAME_MAX);
	if(hostname_ret != 0){
		printf("An error occured trying to retrieve the hostname.\n");
	}
	printf("Host: %s\n", hostname);

	// CURRENT WORKING DIR
	current_working_dir = getcwd(NULL, NULL);
	if(current_working_dir == NULL){
		printf("An error occured trying to retrieve the current working dir.\n");
	}
	printf("Current working dir: %s\n", current_working_dir);

}
