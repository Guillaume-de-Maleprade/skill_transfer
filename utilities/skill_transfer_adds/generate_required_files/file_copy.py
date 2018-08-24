import os
import shutil
import sys

if len(sys.argv) == 2:
    path_to_src = str(sys.argv[1])

    #copy giskard_pr2 files

    src_giskard = "giskard_pr2"
    dest_giskard = path_to_src + "/giskard_pr2/launch"
    giskard_files = os.listdir(src_giskard)
    for file_name in giskard_files:
        full_file_name = os.path.join(src_giskard, file_name)
        if (os.path.isfile(full_file_name)):
            shutil.copy(full_file_name, dest_giskard)

    #copy iai_pr2_sim files

    src_iai = "iai_pr2_sim"
    dest_iai = path_to_src + "/iai_pr2/iai_pr2_sim/launch"
    iai_files = os.listdir(src_iai)
    for file_name in iai_files:
        full_file_name = os.path.join(src_iai, file_name)
        if (os.path.isfile(full_file_name)):
            shutil.copy(full_file_name, dest_iai)

    #copy pr2_gazebo file
    source = open("pr2_gazebo/pr2_pose.launch").read()
    destination = open("/opt/ros/kinetic/share/pr2_gazebo/launch/pr2_pose.launch","w")
    destination.write(source)

else:
    print("use: $ sudo python3 file_copy.py <path/to/src> (e.g. ~/catkin_ws/src )")

