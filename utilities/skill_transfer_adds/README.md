# How to generate the files to use the pr2 with skill_transfer

## Generate the giskard_pr2 and iai_pr2 launch files
In a terminal, from the **generate_required_files** directory :
```bash
$ sudo python3 file_copy.py <path/to/src>
```

### It should be something like :
```bash
$ sudo python3 file_copy.py ~/catkin_ws/src
```

___
## Save the pose of the pr2
Assuming the previous step was done correctly :
```bash
$ roslaunch skill_transfer simulation_giskard.launch world:=scraping_b_big_bowl_b_serving_spoon.world
```
And in another terminal, from inside the **`poses`** directory :
```bash
$ rostopic echo -n 1 joint_states > pose_name.yaml
```
___
## Generate the `pr2_no_controllers_<pose_name>.launch` file
Assuming you have a `pose_name.yaml` file in the **`poses`** folder, in a terminal from **this** directory :
```bash
$ sudo python3 pose_generator.py pose_name "<model_poses>"
```
Chech that the file is at the right path: `/opt/ros/kinetic/share/pr2_gazebo/launch/pr2_no_controllers.launch`. If not, modify the `path` variable in `pose_generator.py`.

#### Example :
```bash
$ sudo python3 pose_generator.py scraping "-x -0.5"
```

___
## Launch the pr2 with the tools stuck to its grippers :
Normally :
```bash
$ roslaunch skill_transfer simulation_pr2.launch world:=scraping_test
```
With markers to control its movements :
```bash
$ roslaunch skill_transfer simulation_giskard.launch world:=scraping_test
```
> All these roslaunch commands work if you insist. Sometimes, you need to **exit** the terminal and open a **new one**.
