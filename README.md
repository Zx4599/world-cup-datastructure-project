1. Read the file teams.txt that contains the teams’ information into a linked list of type 
TEAM.
2. Your program should calculate and store the points for each team as follow
Points = 3*No. of wins + 1*No. of draw
3. After that, you should sort the teams based on the attained points using the Radix Sort.
After sorting, if two teams have the same number of points, the team with greater goals 
difference should appear first. 
4. Your program should support the ability of add, modify and delete any team.
5. Your program should support printing all teams with their relevant information in 
adequate format. 
6. Save back the teams information (sorted) to the file teamsInfo.txt
Phase two
7. The user should have the ability to create a match: Choose two teams (based on their 
teams’ codes) from the previously created list in phase one, if the chosen team is not 
listed an appropriate error message should be shown. 
8. Read the teams’ players lists from the file players.txt. The first 11 players (elementary 
players) should be stored in a linked list of type PLAYER, while the other players (spare 
players) should be stored into a Queue, i.e. you should create one linked list and one 
queue for each team.
9. The team coach has the ability to change the players between elementary and spare; the 
player who is deleted from the elementary should be added to the spare queue.
10. The match referee has the ability to issue the red card to any player; in this case, the 
player should be deleted from the elementary list.
11. Your program should support printing all elementary players for a specific team. 
12. Save back the players information to the file playersInfo.txt
