#include "Mode.hpp"

#include "Scene.hpp"
#include "Sound.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <deque>

struct PlayMode : Mode {
	PlayMode();
	virtual ~PlayMode();

	//functions called by main loop:
	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;

	//----- game state -----

	//input tracking:
	struct Button {
		uint8_t downs = 0;
		uint8_t pressed = 0;
	} left, right, down, up, space, hdown, hup,hleft,hright;

	//local copy of the game scene (so code can change it during gameplay):
	Scene scene;
	std::vector<float> soundrec;
	std::vector<int> simon;
	std::vector<int> player;

	bool simonsays = true;

	int selector = 0;
	//hexapod leg to wobble:
	Scene::Transform *taterbody = nullptr;
	Scene::Transform *taterhead = nullptr;
	Scene::Transform *tateropenmouth = nullptr;
	Scene::Transform *taterclosedmouth = nullptr;
	Scene::Transform *taterplant = nullptr;
	glm::quat tater_head_rotation;
	glm::quat tater_body_rotation;
	glm::quat tater_plant_rotation;


	Scene::Transform *boopsbody = nullptr;
	Scene::Transform *boopshead = nullptr;
	Scene::Transform *boopsopenmouth = nullptr;
	Scene::Transform *boopsopeneye = nullptr;
	Scene::Transform *boopsclosedeye = nullptr;
	Scene::Transform *boopsclosedmouth = nullptr;
	Scene::Transform *boopsRarm = nullptr;
	Scene::Transform *boopsLarm = nullptr;
	Scene::Transform *boopsRankle = nullptr;
	Scene::Transform *boopsLankle = nullptr;
	Scene::Transform *boopstail = nullptr;
	glm::quat boops_head_rotation;
	glm::quat boops_body_rotation;
	glm::quat boops_Rarm_rotation;
	glm::quat boops_Larm_rotation;
	glm::quat boops_Rankle_rotation;
	glm::quat boops_Lankle_rotation;
	glm::quat boops_tail_rotation;
	glm::vec3 boops_body_position;

	Scene::Transform *clydebody = nullptr;
	Scene::Transform *clydehead = nullptr;
	Scene::Transform *clydeopenmouth = nullptr;
	Scene::Transform *clydeRopeneye = nullptr;
	Scene::Transform *clydeLopeneye = nullptr;
	Scene::Transform *clydeRclosedeye = nullptr;
	Scene::Transform *clydeLclosedeye = nullptr;
	Scene::Transform *clydeclosedmouth = nullptr;
	Scene::Transform *clydeReyestalk = nullptr;
	Scene::Transform *clydeLeyestalk  = nullptr;
	Scene::Transform *clydeRarm = nullptr;
	Scene::Transform *clydeLarm = nullptr;
	Scene::Transform *clydeFRleg = nullptr;
	Scene::Transform *clydeFLleg = nullptr;
	Scene::Transform *clydeTRleg = nullptr;
	Scene::Transform *clydeTLleg = nullptr;
	Scene::Transform *clydeBRleg = nullptr;
	Scene::Transform *clydeBLleg = nullptr;
	glm::quat clyde_head_rotation;
	glm::quat clyde_body_rotation;
	glm::quat clyde_Rarm_rotation;
	glm::quat clyde_Larm_rotation;
	glm::quat clyde_Reyestalk_rotation;
	glm::quat clyde_Leyestalk_rotation;
	glm::quat clyde_FRleg_rotation;
	glm::quat clyde_FLleg_rotation;
	glm::quat clyde_TRleg_rotation;
	glm::quat clyde_TLleg_rotation;
	glm::quat clyde_BRleg_rotation;
	glm::quat clyde_BLleg_rotation;
	glm::vec3 clyde_body_position;

	float wobble = 0.0f;
	float tatersquash = -1.0f;
	float boopssquash = -1.0f;
	float clydesquash = -1.0f;

	glm::vec3 get_tater_head_tip_position();
	glm::vec3 get_boops_head_tip_position();
	glm::vec3 get_clyde_head_tip_position();


	float timer = 1.0f;
	float selectortimer = 0.5f;


	//music coming from the tip of the leg (as a demonstration):
	std::shared_ptr< Sound::PlayingSample > tater_head_tip_loop;
	std::shared_ptr< Sound::PlayingSample > boops_head_tip_loop;
	std::shared_ptr< Sound::PlayingSample > clyde_head_tip_loop;
	
	//camera:
	Scene::Camera *camera = nullptr;

};
