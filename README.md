# Trainmepar
This is Railway managment system using socket programming ,multi client-server concept and file  Locking protocol

## Compilation:

1. Go to the project location
2. Type $ make
3. Type $ make clean
4. The above will generate server.out and client.out

### Server:

1. Prompts each user for login
2. Maintains user login record and creates entry for new user

### Client:

1. Admin:
	*	Adds train
	*	Edits details for an existing train
	*	Lists details of existing user
	*	Delete existing user

2. User/Agent:
	*	Creates new user
	*	Books train from given source and destination
	*	Lists the already booked trains
	*	Cancels the booked train