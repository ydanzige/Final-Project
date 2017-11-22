# C++ Final project: Chatter
It’s the nineties all over again, and everyone’s writing their own instant
messaging app. If it worked for ICQ, MSN, and Facebook, why not you?
Create a client/server messaging app. The following features should be
supported:
1. Login: Using a server name and port, and a user name. Names that are
currently already in use should be rejected.
2. List all currently active users.
3. Send a message to a single user.
4. Send a message to all users.
5. Ban a user – do not receive further message from them.
6. Unban a user – new messages should be received and displayed.
7. All messages should be relayed via the server; no direct communication
between clients.
8. Use the Microsoft C++ REST SDK library for communications.
Instructions:
1. The class will be split into two project teams, each implementing its own
product.
2. Each project comprises the following structure:
a. Server business logic team: Manages state, incoming and
outgoing messages, etc.
b. Server communications team: Manages network protocol and
transmissions. Should work closely with the client
communications team.
c. Client front-end team: Manages application and UI (which should
be a command-line interface). Works closely with server
communications team.
d. Client communications team: Communicates with the server.
3. Each team is assigned two members (pair programming).
4. Each project will have a project manager, who is also a team member in
one of the teams. In addition to their team assignments, project
managers will be responsible for their project’s GitHub repo, resolving
conflicts, etc.
5. Project members should establish interfaces as soon as possible and
implement “empty” implementations, allowing other teams to start
interacting with their interfaces.
6. Remember to commit and push only working (i.e. compiled and tested)
code, as other project members will be pulling and merging from your
code.
7. Establish a working procedure for Git (e.g. Git Flow).
8. Divide the project into milestones/iterations, e.g. hourly merges.
Good luck!
