#include "PlayMode.hpp"

#include "LitColorTextureProgram.hpp"

#include "DrawLines.hpp"
#include "Mesh.hpp"
#include "Load.hpp"
#include "gl_errors.hpp"
#include "data_path.hpp"

#include <glm/gtc/type_ptr.hpp>

#include <random>

GLuint alien_meshes_for_lit_color_texture_program = 0;
Load< MeshBuffer > alien_meshes(LoadTagDefault, []() -> MeshBuffer const * {
	MeshBuffer const *ret = new MeshBuffer(data_path("alien.pnct"));
	alien_meshes_for_lit_color_texture_program = ret->make_vao_for_program(lit_color_texture_program->program);
	return ret;
});

Load< Scene > alien_scene(LoadTagDefault, []() -> Scene const * {
	return new Scene(data_path("alien.scene"), [&](Scene &scene, Scene::Transform *transform, std::string const &mesh_name){
		Mesh const &mesh = alien_meshes->lookup(mesh_name);
		std::cout <<mesh_name;
		std::cout << "\n";
		std::string helmetname = "Helmet";
		if(!(mesh_name.find(helmetname)))
		{
			
			scene.drawables.emplace_front(transform);
			Scene::Drawable &drawable = scene.drawables.front();
			drawable.pipeline = lit_color_texture_program_pipeline;
			
			drawable.pipeline.vao = alien_meshes_for_lit_color_texture_program;
			drawable.pipeline.type = mesh.type;
			drawable.pipeline.start = mesh.start;
			drawable.pipeline.count = mesh.count;

			drawable.pipeline.trans = 1; //set transparency to 1
		}
		else
		{
			scene.drawables.emplace_back(transform);
			Scene::Drawable &drawable = scene.drawables.back();
			drawable.pipeline = lit_color_texture_program_pipeline;
			drawable.pipeline.vao = alien_meshes_for_lit_color_texture_program;
			drawable.pipeline.type = mesh.type;
			drawable.pipeline.start = mesh.start;
			drawable.pipeline.count = mesh.count;
		}

		

	});
});

Load< Sound::Sample > dusty_floor_sample(LoadTagDefault, []() -> Sound::Sample const * {
	return new Sound::Sample(data_path("dusty-floor.opus"));
});

Load< Sound::Sample > tatercry(LoadTagDefault, []() -> Sound::Sample const * {
	return new Sound::Sample(data_path("Pickup_coin 25v2.wav"));
});

Load< Sound::Sample > boopscry(LoadTagDefault, []() -> Sound::Sample const * {
	return new Sound::Sample(data_path("Laser_shoot 14.wav"));
});


Load< Sound::Sample > clydecry(LoadTagDefault, []() -> Sound::Sample const * {
	return new Sound::Sample(data_path("clydecry.wav"));
});

Load< Sound::Sample > ambience(LoadTagDefault, []() -> Sound::Sample const * {
	return new Sound::Sample(data_path("Space Ambience.wav"));
});



PlayMode::PlayMode() : scene(*alien_scene) {
	//get pointers to leg for convenience:
	for (auto &transform : scene.transforms) {
		if (transform.name == "taterHead") taterhead = &transform;
		else if (transform.name == "taterBody") taterbody = &transform;
		else if (transform.name == "taterStem") taterplant = &transform;
		else if (transform.name == "taterClosedMouth") taterclosedmouth = &transform;
		else if (transform.name == "taterOpenMouth") tateropenmouth = &transform;
		else if (transform.name == "boopsBody") boopsbody = &transform;
		else if (transform.name == "boopsHead") boopshead = &transform;
		else if (transform.name == "boopsClosedMouth") boopsclosedmouth = &transform;
		else if (transform.name == "boopsOpenMouth") boopsopenmouth = &transform;
		else if (transform.name == "boopsRArm") boopsRarm = &transform;
		else if (transform.name == "boopsLArm") boopsLarm = &transform;
		else if (transform.name == "boopsRAnkle") boopsRankle = &transform;
		else if (transform.name == "boopsLAnkle") boopsLankle = &transform;
		else if (transform.name == "boopsCEyes") boopsclosedeye = &transform;
		else if (transform.name == "boopsOEyes") boopsopeneye = &transform;
		else if (transform.name == "boopsTail") boopstail = &transform;
		else if (transform.name == "clydebody") clydebody = &transform;
		else if (transform.name == "clydehead") clydehead = &transform;
		else if (transform.name == "clydeclosedmouth") clydeclosedmouth = &transform;
		else if (transform.name == "clydemouthopen") clydeopenmouth = &transform;
		else if (transform.name == "clydeRarm") clydeRarm = &transform;
		else if (transform.name == "clydeLarm") clydeLarm = &transform;
		else if (transform.name == "clydeFRleg") clydeFRleg = &transform;
		else if (transform.name == "clydeFLleg") clydeFLleg = &transform;
		else if (transform.name == "clydeTRleg") clydeTRleg = &transform;
		else if (transform.name == "clydeTLleg") clydeTLleg = &transform;
		else if (transform.name == "clydeBRleg") clydeBRleg = &transform;
		else if (transform.name == "clydeBLleg") clydeBLleg = &transform;
		else if (transform.name == "clydeReyeclosed") clydeRclosedeye = &transform;
		else if (transform.name == "clydeLeyeclosed") clydeLclosedeye = &transform;
		else if (transform.name == "clydeRopeneye") clydeRopeneye = &transform;
		else if (transform.name == "clydeLopeneye") clydeLopeneye = &transform;
		else if (transform.name == "clydeReyestalk") clydeReyestalk = &transform;
		else if (transform.name == "clydeLeyestalk") clydeLeyestalk= &transform;
	}
	tater_head_rotation = taterhead->rotation;
	tater_body_rotation = taterbody->rotation;
	tater_plant_rotation = taterplant->rotation;
	tateropenmouth -> scale = glm::vec3(0.0f, 0.0f, 0.0f);


	boops_body_rotation = boopsbody->rotation;
	boops_head_rotation = boopshead->rotation;
	boops_Rarm_rotation = boopsRarm->rotation;
	boops_Larm_rotation = boopsLarm->rotation;
	boops_Rankle_rotation = boopsRankle->rotation;
	boops_Lankle_rotation = boopsLankle->rotation;
	boops_tail_rotation = boopstail->rotation;
	boopsopenmouth -> scale = glm::vec3(0.0f, 0.0f, 0.0f);
	boopsclosedeye -> scale = glm::vec3(0.0f, 0.0f, 0.0f);
	boops_body_position = boopsbody -> position;


	clyde_body_rotation = clydebody->rotation;
	clyde_head_rotation = clydehead->rotation;
	clyde_Rarm_rotation = clydeRarm->rotation;
	clyde_Larm_rotation = clydeLarm->rotation;
	clyde_FRleg_rotation = clydeFRleg->rotation;
	clyde_FLleg_rotation = clydeFLleg->rotation;
	clyde_TRleg_rotation = clydeTRleg->rotation;
	clyde_TLleg_rotation = clydeTLleg->rotation;
	clyde_BRleg_rotation = clydeBRleg->rotation;
	clyde_BLleg_rotation = clydeBLleg->rotation;
	clyde_Reyestalk_rotation = clydeReyestalk->rotation;
	clyde_Leyestalk_rotation = clydeLeyestalk->rotation;
	clydeopenmouth -> scale = glm::vec3(0.0f, 0.0f, 0.0f);
	clydeRclosedeye -> scale = glm::vec3(0.0f, 0.0f, 0.0f);
	clydeLclosedeye -> scale = glm::vec3(0.0f, 0.0f, 0.0f);
	clyde_body_position = clydebody -> position;


	//get pointer to camera for convenience:
	if (scene.cameras.size() != 1) throw std::runtime_error("Expecting scene to have exactly one camera, but it has " + std::to_string(scene.cameras.size()));
	camera = &scene.cameras.front();
	tater_head_tip_loop = Sound::play_3D(*tatercry, 0.0f, get_tater_head_tip_position(), 10.0f);
	boops_head_tip_loop = Sound::play_3D(*boopscry, 0.0f, get_boops_head_tip_position(), 10.0f);
	clyde_head_tip_loop = Sound::play_3D(*clydecry, 0.0f, get_clyde_head_tip_position(), 10.0f);
	Sound::loop(*ambience, 0.25f, 0.0f);
	//start music loop playing:
	// (note: position will be over-ridden in update())
	
}

PlayMode::~PlayMode() {
}

bool PlayMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {

	if (evt.type == SDL_KEYDOWN) {
		if (evt.key.keysym.sym == SDLK_ESCAPE) {
			SDL_SetRelativeMouseMode(SDL_FALSE);
			return true;
		} else if (evt.key.keysym.sym == SDLK_a) {
			left.downs += 1;
			left.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_d) {
			right.downs += 1;
			right.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_w) {
			up.downs += 1;
			up.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_s) {
			down.downs += 1;
			down.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_UP) {
			hup.downs += 1;
			hup.pressed = true;
			return true;
		}
		else if (evt.key.keysym.sym == SDLK_DOWN) {
			hdown.downs += 1;
			hdown.pressed = true;
			return true;
		}
		else if (evt.key.keysym.sym == SDLK_LEFT) {
			hleft.downs += 1;
			hleft.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_RIGHT) {
			hright.downs += 1;
			hright.pressed = true;
			return true;
		}else if (evt.key.keysym.sym == SDLK_SPACE) {
			space.downs += 1;
			space.pressed = true;
			return true;
		}
	} else if (evt.type == SDL_KEYUP) {
		if (evt.key.keysym.sym == SDLK_a) {
			left.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_d) {
			right.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_w) {
			up.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_s) {
			down.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_UP) {
			hup.pressed = false;
			return true;
		}
		else if (evt.key.keysym.sym == SDLK_DOWN) {
			hdown.pressed = false;
			return true;
		}
		 else if (evt.key.keysym.sym == SDLK_LEFT) {
			hleft.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_RIGHT) {
			hright.pressed = false;
			return true;
		}
		else if (evt.key.keysym.sym == SDLK_SPACE) {
			space.pressed = false;
			return true;
		}
	} else if (evt.type == SDL_MOUSEBUTTONDOWN) {
		if (SDL_GetRelativeMouseMode() == SDL_FALSE) {
			SDL_SetRelativeMouseMode(SDL_TRUE);
			return true;
		}
	}

	return false;
}

void tater_squeak(float &tatersquash, float elapsed, Scene::Transform *closedmouth, Scene::Transform *openmouth, Scene::Transform *body)
{
	if(tatersquash >= 0.0f) //tatersquash animation
	{
		tatersquash -= elapsed;
		openmouth -> scale = glm::vec3(1.0f, 1.0f, 1.0f);
		closedmouth -> scale = glm::vec3(0.0f, 0.0f, 0.0f);
		body -> scale.y = 0.25f * abs(sin( float(M_PI) *tatersquash * 1.5f)) + 0.75f;
		if(tatersquash < 2.25f)
		{
			tatersquash = -1.0f;
			openmouth -> scale = glm::vec3(0.0f, 0.0f, 0.0f);
			closedmouth -> scale = glm::vec3(1.0f, 1.0f, 1.0f);
		}
		
	}

}

void boops_squeak(float &boopssquash, float elapsed, Scene::Transform *closedmouth, Scene::Transform 
				*openmouth,Scene::Transform *closedeye, Scene::Transform *openeye, Scene::Transform *body,
				Scene::Transform *Rarm, Scene::Transform *Larm, glm::quat Rarm_rotation,glm::quat Larm_rotation )
{
		if(boopssquash >= 0.0f) //boopssquash animation
	{
		boopssquash -= elapsed;
		openmouth -> scale = glm::vec3(1.0f, 1.0f, 1.0f);
		closedmouth -> scale = glm::vec3(0.0f, 0.0f, 0.0f);
		openeye -> scale = glm::vec3(0.0f, 0.0f, 0.0f);
		closedeye -> scale = glm::vec3(1.0f, 1.0f, 1.0f);
		body -> scale.y = 0.125f * abs(sin( float(M_PI) *boopssquash * 1.5f)) + 1.0f;
		Rarm -> rotation = Rarm_rotation * glm::angleAxis(
		glm::radians((-15.0f * std::sin(boopssquash * 2.0f  * float(M_PI)))+65.0f),
		glm::vec3(0.0f, 0.0f, 1.0f ));
		Larm -> rotation = Larm_rotation * glm::angleAxis(
		glm::radians((-15.0f * std::sin(boopssquash * -2.0f  * float(M_PI)))-65.0f),
		glm::vec3(0.0f, 0.0f, 1.0f ));

		if(boopssquash <= 1.33f)
		{
			
			boopssquash = -1.0f;
			openmouth -> scale = glm::vec3(0.0f, 0.0f, 0.0f);
			closedmouth -> scale = glm::vec3(1.0f, 1.0f, 1.0f);
			openeye-> scale = glm::vec3(1.0f, 1.0f, 1.0f);
			closedeye-> scale = glm::vec3(0.0f, 0.0f, 0.0f);
		}
		
	}
}

void clyde_squeak(float &clydesquash, float elapsed, Scene::Transform *closedmouth, Scene::Transform 
				*openmouth,Scene::Transform *Rclosedeye, Scene::Transform *Lclosedeye,  Scene::Transform *Ropeneye, Scene::Transform *Lopeneye, Scene::Transform *body,
				Scene::Transform *Rarm, Scene::Transform *Larm, glm::quat Rarm_rotation,glm::quat Larm_rotation )
{
		if(clydesquash >= 0.0f) //clydesquash animation
	{
		clydesquash -= elapsed;
		openmouth -> scale = glm::vec3(1.0f, 1.0f, 1.0f);
		closedmouth -> scale = glm::vec3(0.0f, 0.0f, 0.0f);
		Ropeneye -> scale = glm::vec3(0.0f, 0.0f, 0.0f);
		Lopeneye -> scale = glm::vec3(0.0f, 0.0f, 0.0f);
		Rclosedeye -> scale = glm::vec3(1.0f, 1.0f, 1.0f);
		Lclosedeye -> scale = glm::vec3(1.0f, 1.0f, 1.0f);
		body -> scale.y = 0.25f * abs(sin( float(M_PI) *clydesquash * 1.5f)) + 0.75f;

		if(clydesquash <= 2.25f)
		{
			
			clydesquash = -1.0f;
			openmouth -> scale = glm::vec3(0.0f, 0.0f, 0.0f);
			closedmouth -> scale = glm::vec3(1.0f, 1.0f, 1.0f);
			Ropeneye-> scale = glm::vec3(1.0f, 1.0f, 1.0f);
			Lopeneye-> scale = glm::vec3(1.0f, 1.0f, 1.0f);
			Rclosedeye-> scale = glm::vec3(0.0f, 0.0f, 0.0f);
			Lclosedeye-> scale = glm::vec3(0.0f, 0.0f, 0.0f);
		}
		
	}
}



void PlayMode::update(float elapsed) {

	wobble += elapsed / 10.0f;
	timer -= elapsed;
	selectortimer -= elapsed;

	if(hleft.pressed && (selector > 0) && selectortimer <= 0.0f)
	{
		selector -= 1;
		selectortimer = 0.5f;
	}
	
	if(hright.pressed && (selector < 2) && selectortimer <= 0.0f)
	{
		selector += 1;
		selectortimer = 0.5f;
	}

	
	//movtimer += elapsed;
	/*if(record) //if it is recording increment the record timer 
	rectime += elapsed;
	
	if(!record) //if it is not recording increment the timer
	timer += elapsed; */

	
	
	//tater idle

	taterbody -> rotation = tater_body_rotation * glm::angleAxis(
		glm::radians(5.0f * std::sin(wobble * 5.0f  * float(M_PI))),
		glm::vec3(1.0f, 0.0f, 0.0f ));
	taterplant -> rotation = tater_plant_rotation * glm::angleAxis(
		glm::radians(15.0f * std::sin(wobble * 5.0f  * float(M_PI))),
		glm::vec3(1.0f, 0.0f, 0.0f ));


	//boops idle
	boopsRarm -> rotation = boops_Rarm_rotation * glm::angleAxis(
		glm::radians(5.0f * std::sin(wobble * 5.0f  * float(M_PI))),
		glm::vec3(0.0f, 0.0f, 1.0f ));

	boopsLarm -> rotation = boops_Larm_rotation * glm::angleAxis(
		glm::radians(5.0f * std::sin(wobble * -5.0f  * float(M_PI))),
		glm::vec3(0.0f, 0.0f, 1.0f ));

	boopsRankle -> rotation = boops_Rankle_rotation * glm::angleAxis(
		glm::radians(10.0f * std::sin(wobble * 5.0f  * float(M_PI))),
		glm::vec3(0.0f, 0.0f, 1.0f ));

	boopsLankle -> rotation = boops_Lankle_rotation * glm::angleAxis(
		glm::radians(10.0f * std::sin(wobble * -5.0f  * float(M_PI))),
		glm::vec3(0.0f, 0.0f, 1.0f ));

	boopstail -> rotation = boops_tail_rotation * glm::angleAxis(
		glm::radians((50.0f * std::sin(wobble * 7.5f  * float(M_PI))) - 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f ));

	boopsbody -> position = boops_body_position  * glm::angleAxis(
		glm::radians(std::sin(wobble * -5.0f  * float(M_PI))),
		glm::vec3(0.0f, 0.0f, 1.0f ));


	//clyde idle

	clydeRarm -> rotation = clyde_Rarm_rotation * glm::angleAxis(
		glm::radians(5.0f * std::sin(wobble * 5.0f  * float(M_PI))),
		glm::vec3(1.0f, 0.0f, 0.0f ));

	clydeLarm -> rotation = clyde_Larm_rotation * glm::angleAxis(
		glm::radians(5.0f * std::sin(wobble * -5.0f  * float(M_PI))),
		glm::vec3(1.0f, 0.0f, 0.0f ));

	clydeReyestalk -> rotation = clyde_Reyestalk_rotation * glm::angleAxis(
		glm::radians(5.0f * std::sin(wobble * 5.0f  * float(M_PI))),
		glm::vec3(0.0f, 0.0f, 1.0f ));

	clydeLeyestalk-> rotation = clyde_Leyestalk_rotation * glm::angleAxis(
		glm::radians(5.0f * std::sin(wobble * -5.0f  * float(M_PI))),
		glm::vec3(0.0f, 0.0f, 1.0f ));

	clydeFRleg -> rotation = clyde_FRleg_rotation * glm::angleAxis(
		glm::radians(5.0f * std::sin(wobble * 5.0f  * float(M_PI))),
		glm::vec3(0.0f, 0.0f, 1.0f ));

	clydeFLleg -> rotation = clyde_FLleg_rotation * glm::angleAxis(
		glm::radians(5.0f * std::sin(wobble * -5.0f  * float(M_PI))),
		glm::vec3(0.0f, 0.0f, 1.0f ));


	clydeTRleg -> rotation = clyde_TRleg_rotation * glm::angleAxis(
		glm::radians(5.0f * std::sin(wobble * 5.0f  * float(M_PI))),
		glm::vec3(0.0f, 0.0f, 1.0f ));

	clydeTLleg -> rotation = clyde_TLleg_rotation * glm::angleAxis(
		glm::radians(5.0f * std::sin(wobble * -5.0f  * float(M_PI))),
		glm::vec3(0.0f, 0.0f, 1.0f ));

	clydeBRleg -> rotation = clyde_BRleg_rotation * glm::angleAxis(
		glm::radians(5.0f * std::sin(wobble * 5.0f  * float(M_PI))),
		glm::vec3(0.0f, 0.0f, 1.0f ));

	clydeBLleg -> rotation = clyde_BLleg_rotation * glm::angleAxis(
		glm::radians(5.0f * std::sin(wobble * -5.0f  * float(M_PI))),
		glm::vec3(0.0f, 0.0f, 1.0f ));

	clydebody -> position = clyde_body_position  * glm::angleAxis(
		glm::radians(std::sin(wobble * -5.0f  * float(M_PI))),
		glm::vec3(0.0f, 0.0f, 1.0f ));


	/*if(!record && !soundrec.empty()) //if the record button is not pressed and soundrec is not empty
	{
		
		auto it = soundrec.begin(); //iterator that points to the first element
		if(timer >= *it) //if timer is greater than or equal to this element
		{
			tater_head_tip_loop = Sound::play_3D(*tatercry, 1.0f, get_tater_head_tip_position(), 10.0f); //play the sound
			float sendback = *it;
			soundrec.erase(it);
			soundrec.emplace_back(sendback); //erase the time from the front and send it to the back
			timer = 0.0f;
			tatersquash = 2.5f;
		}
		

	} */

	if(simonsays && player.empty())
	{
		simon.emplace_back(rand()%3);
		simonsays = false;
		timer = 1.0f;
	}
	else if(!simon.empty() && !simonsays) //if the simon says vectoris not empty, we still have to play the sounds
	{
		auto it = simon.begin();
		if(timer <= 0.0f) //if the timer has run out
		{
			if(*it == 0)
			{
				clyde_head_tip_loop = Sound::play_3D(*clydecry, 1.0f, get_clyde_head_tip_position(), 10.0f); //play the sound
				clydesquash = 2.5f;
			}
			else if(*it == 1)
			{
				boops_head_tip_loop = Sound::play_3D(*boopscry, 1.0f, get_boops_head_tip_position(), 10.0f); //play the sound
				boopssquash = 2.0f;
			}
			else if(*it == 2)
			{
				tater_head_tip_loop = Sound::play_3D(*tatercry, 1.0f, get_tater_head_tip_position(), 10.0f); //play the sound
				tatersquash = 2.5f;

			}
			
			
			int sendback = *it;
			simon.erase(it);
			player.emplace_back(sendback); //erase the time from the front and send it to the back
			timer = 1.0f;
			
		}

	}
	else //let the player guess the input
	{
		simonsays = true;
		if(space.pressed && tatersquash == -1.0f && boopssquash == -1.0f && clydesquash == -1.0f) //if you squish the creature
		{
			auto it = player.begin();
			
			if(*it == 0)
			{
				clyde_head_tip_loop = Sound::play_3D(*clydecry, 1.0f, get_clyde_head_tip_position(), 10.0f); //play the sound
				clydesquash = 2.5f;
			}
			else if(*it == 1)
			{
				boopssquash = 2.0f;
				boops_head_tip_loop = Sound::play_3D(*boopscry, 1.0f, get_boops_head_tip_position(), 10.0f);
			}
			else if(*it == 2)
			{	
				tatersquash = 2.5f;
				tater_head_tip_loop = Sound::play_3D(*tatercry, 1.0f, get_tater_head_tip_position(), 10.0f);
			}

			if(selector == *it) //if the player is right continue on
			{
				int sendback = *it;
				player.erase(it);
				simon.emplace_back(sendback); //erase the time from the front and send it to the back
			}
			else //if the player is wrong then start over
			{
				player.clear();
				simon.clear();
			}
			
		}
		
	}

	
	

	tater_squeak(tatersquash,elapsed,taterclosedmouth,tateropenmouth,taterbody);
	
	boops_squeak(boopssquash,elapsed,boopsclosedmouth,boopsopenmouth,boopsclosedeye,boopsopeneye,boopsbody, boopsRarm, boopsLarm, boops_Rarm_rotation, boops_Larm_rotation);
	
	clyde_squeak(clydesquash,elapsed,clydeclosedmouth,clydeopenmouth,clydeLclosedeye ,clydeRclosedeye,clydeRopeneye, clydeLopeneye,clydebody, clydeRarm, clydeLarm, clyde_Rarm_rotation, clyde_Larm_rotation);
	//move sound to follow leg tip position:
	tater_head_tip_loop->set_position(get_tater_head_tip_position(), 1.0f / 60.0f); 

	boops_head_tip_loop->set_position(get_boops_head_tip_position(), 1.0f / 60.0f); 

	clyde_head_tip_loop->set_position(get_clyde_head_tip_position(), 1.0f / 60.0f); 
	//move camera:
	{ //update listener to camera position:
		glm::mat4x3 frame = camera->transform->make_local_to_parent();
		glm::vec3 frame_right = frame[0];
		glm::vec3 frame_at = frame[3];
		Sound::listener.set_position_right(frame_at, frame_right, 1.0f / 60.0f);
	}


	//if 1 is pressed create a new monster 


	//reset button press counters:
	left.downs = 0;
	right.downs = 0;
	up.downs = 0;
	down.downs = 0;
	space.downs = 0;
	hdown.downs = 0;
	hup.downs = 0;
	hleft.downs = 0;
	hright.downs = 0;
}

void PlayMode::draw(glm::uvec2 const &drawable_size) {
	//update camera aspect ratio for drawable:
	camera->aspect = float(drawable_size.x) / float(drawable_size.y);

	//set up light type and position for lit_color_texture_program:
	// TODO: consider using the Light(s) in the scene to do this
	glUseProgram(lit_color_texture_program->program);
	glUniform1i(lit_color_texture_program->LIGHT_TYPE_int, 1);
	glUniform3fv(lit_color_texture_program->LIGHT_DIRECTION_vec3, 1, glm::value_ptr(glm::vec3(0.0f, -1.0f,1.0f)));
	glUniform3fv(lit_color_texture_program->LIGHT_ENERGY_vec3, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 0.95f)));
	glUseProgram(0);


	glUseProgram(lit_color_texture_program->flatprogram);
	glUniform1i(lit_color_texture_program->LIGHT_TYPE_int, 1);
	glUniform3fv(lit_color_texture_program->LIGHT_DIRECTION_vec3, 1, glm::value_ptr(glm::vec3(-1.0f, -1.0f,1.0f)));
	glUniform3fv(lit_color_texture_program->LIGHT_ENERGY_vec3, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 0.95f)));
	glUseProgram(0);

	
	glClearColor(0.02f, 0.02f, 0.2f, 1.0f);
	glClearDepth(1.0f); //1.0 is actually the default value to clear the depth buffer to, but FYI you can change it.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); //this is the default depth comparison function, but FYI you can change it.

	scene.draw(*camera);

	
	GL_ERRORS();
}

glm::vec3 PlayMode::get_tater_head_tip_position() {
	//the vertex position here was read from the model in blender:
	return taterhead->make_local_to_world() * glm::vec4(1.98215f, 1.73985f, 0.0f, 1.0f);
}


glm::vec3 PlayMode::get_boops_head_tip_position() {
	//the vertex position here was read from the model in blender:
	return boopshead->make_local_to_world() * glm::vec4(0.0f, 2.2f, 2.2f, 1.0f);
}

glm::vec3 PlayMode::get_clyde_head_tip_position() {
	//the vertex position here was read from the model in blender:
	return clydehead->make_local_to_world() * glm::vec4(0.0f, 1.0f, 2.0f, 1.0f);
}



