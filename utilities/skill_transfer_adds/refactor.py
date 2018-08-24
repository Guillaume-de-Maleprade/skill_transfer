# Refactor 'pr2_no_controllers' if it has been broken

temp = open("pr2_no_controllers_template.launch").read()
rep = open("/opt/ros/kinetic/share/pr2_gazebo/launch/pr2_no_controllers.launch", 'w')
rep.write(temp)