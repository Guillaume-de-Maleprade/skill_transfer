//#include "GripTest.hh"

#include <gazebo_msgs/GetModelState.h>
#include <ros/ros.h>
#include <thread> // std::this_thread::sleep_for
#include <chrono> // std::chrono::seconds
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <stdio.h>

namespace gazebo
{
class GripTool : public WorldPlugin
{

  public:
    void Load(physics::WorldPtr _world, sdf::ElementPtr _sdf)
    {

        this->model = _sdf;
        this->world = _world;
        this->physics = this->world->GetPhysicsEngine();

        this->childLinkName = this->model->GetElement("childLinkName")->Get<std::string>();
        this->parentLinkName = this->model->GetElement("parentLinkName")->Get<std::string>(); // [r|l]_gripper_palm_joint

        this->client = n.serviceClient<gazebo_msgs::GetModelState>("/gazebo/get_model_state");

        this->updateConnection = event::Events::ConnectWorldUpdateBegin(
            boost::bind(&GripTool::OnUpdate, this, _1));
        this->i = 0;
    }

  public:
    void OnUpdate(const common::UpdateInfo &_info)
    {
        if (_info.simTime < 2.0)
        {
            // Let the stage settle down and position objects
            return;
        }

        if (!this->ok)
        {
            gazebo_msgs::GetModelState srv;
            srv.request.model_name = "pr2";
            i++;

            // Check if the pr2 has spawned
            if (this->client.call(srv))
            {
                this->ok = true;
                ROS_INFO("Success : %d", srv.response.success);
            }
            else
            {
                ROS_INFO("Failed to call service get_model_state of pr2");
                // return 1;
            }
        }
        else
        {
            // If the pr2 has spawn and the tools aren't stuck to the grippers
            if (!this->done)
            {
                this->done = true;
                math::Pose relativePose;

                // Calculate the relativePose
                if (this->model->HasElement("relativePose"))
                {

                    relativePose = this->model->GetElement("relativePose")->Get<math::Pose>();

                    this->parentLink = boost::dynamic_pointer_cast<physics::Link>(this->world->GetEntity(this->parentLinkName));

                    this->childLink = boost::dynamic_pointer_cast<physics::Link>(this->world->GetEntity(this->childLinkName));

                    math::Pose parentPose = this->parentLink->GetWorldPose();

                    math::Pose childPose = math::Pose(parentPose.pos + (parentPose.rot.RotateVector(relativePose.pos)), parentPose.rot * relativePose.rot);

                    this->childLink->SetWorldPose(childPose);

                    gzdbg << "Grip: Relative pose given, adjusting child pose\n"
                          << childPose << "\n";
                }
                else
                {
                    relativePose = this->parentLink->GetWorldPose() - this->childLink->GetWorldPose();

                    gzdbg << "Grip: Relative pose derrived\n";
                }

                // Create joint
                const auto joint = physics->CreateJoint("fixed", parentModel);
                // Bullet physics needs accurate joint position
                // ODE does't care
                joint->Load(parentLink, childLink, relativePose);
                joint->Init();
                joint->SetName("grip_joint_" + parentLink->GetScopedName() + "_" + childLink->GetScopedName());

                this->childLink->SetGravityMode(false);
            }
        }
    }

  private:
    bool ok = false;
    bool done = false;
    physics::PhysicsEnginePtr physics;
    physics::LinkPtr parentLink;
    physics::LinkPtr childLink;
    sdf::ElementPtr model;
    physics::ModelPtr parentModel;
    physics::WorldPtr world;
    std::string childLinkName;
    std::string parentLinkName;
    event::ConnectionPtr updateConnection;
    ros::NodeHandle n;
    ros::ServiceClient client;
    int i;

}; // Register this plugin with the simulator
GZ_REGISTER_WORLD_PLUGIN(GripTool);
} // namespace gazebo
