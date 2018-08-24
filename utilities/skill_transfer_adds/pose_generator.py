import yaml
import sys
path = "/opt/ros/kinetic/share/pr2_gazebo/launch/pr2_no_controllers"

if len(sys.argv) == 3:
    pos = str(sys.argv[1])

    yfile = open("poses/"+pos+".yaml").read()
    if yfile.endswith("---\n"):
        yfile = str(yfile)[:-4]


    y = yaml.load(yfile)

    pos = str(sys.argv[1])

    # file = open("poses/"+pos+".txt","w")

    pose_str = ""

    i=0

    for ind in y["name"]:
        #print("-J " + ind + " " + str(y["position"][i]))
        if abs(y["position"][i]) > 0.05:
            # file.write("-J " + ind + " " + str(y["position"][i]) +"\n")
            pose_str += "-J " + ind + " " + str(y["position"][i]) +"\n"
        i=i+1

    print(pose_str)




    # string = open("poses/"+str(sys.argv[1])+".txt").read()
    pose_str = str(sys.argv[2]) + ' ' + pose_str
    s = open(path + ".launch").read() 
    s = s.replace('$(optenv ROBOT_INITIAL_POSE)', str(pose_str))
    f = open(path+"_"+str(sys.argv[1])+".launch", 'w')
    f.write(s)
    f.close()
else:
    print("use : sudo python3 pose_generator.py <pose_name> \"model_poses\" (e. g. \"-x -0.5\")")