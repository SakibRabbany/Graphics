-- A simple scene with some miscellaneous geometry.

-- mat1 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 25, {0, 1000, 0, 0}, 0.27)
-- mat2 = gr.material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, 25, {0, 0, 0, 0}, 0.05)
-- mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 25, {0, 0, 0, 0}, 2.41)
-- mat4 = gr.material({0.7, 0.6, 1.0}, {0.5, 0.4, 0.8}, 25, {0, 0, 0, 0}, 0.27)
-- mat5 = gr.material({0.7, 0.6, 1.0}, {0.5, 0.4, 0.8}, 25, {0, 1000, 0, 0}, 0.27)

red_wall = gr.material( {0.44, 0.137, 0.1886}, {0, 0, 0}, 25, {0,0,0,0}, 0.0)
green_wall = gr.material( {0.1386, 0.1327, 0.45}, {0, 0, 0}, 25,{0,0,0,0}, 0.0)
grey_wall = gr.material( {0.4649, 0.4823, 0.5002}, {0, 0, 0}, 25,{0,0,0,0}, 0.0)
light_wall = gr.material( {0.223, 0.498, 0.858}, {0, 0, 0}, 25,{0,0,0,0}, 0.0)
brown = gr.material( {0.203, 0.356, 0.184}, {0, 0, 0}, 25,{0,0,0,0}, 0.0)
red = gr.material( {0.835, 0.141, 0.101}, {0, 0, 0}, 25,{0,0,0,0}, 0.0)
white = gr.material( {1, 1, 1}, {0, 0, 0}, 25,{0,0,0,0}, 0.0)
table = gr.material( {0.113, 0.988, 0.580}, {0, 0, 0}, 25,{0,0,0,0}, 0.0)
chair_back = gr.material( {0.149, 0.309, 0.235}, {0.2, 0.2, 0.2}, 25,{0,0,0,0}, 0.0)

blackish = gr.material( {0.035, 0.019, 0.062}, {0, 0, 0}, 25,{0,0,0,0}, 0.0)

magenta = gr.material( {0.835, 0.101, 0.698}, {0, 0, 0}, 25,{0,0,0,0}, 0.0)
mat3  = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 25,{0,0,0,0}, 0.0)	
mat4  = gr.material({0.7, 0.6, 1.0}, {0.5, 0.4, 0.8}, 25,{0,0,0,0}, 0.0)		


mirror = gr.material( {0.0, 0.0, 0.0},   {1.0, 1.0, 1.0},	25,{0,0,0,0}, 0.0)

glass = gr.material( {0.05, 0.05, 0.05}, {0.7, 0.7, 0.7}, 25, {0,0,0,0}, 1.5)	

scene_root = gr.node('root')


backwall = gr.nh_box('backwall', {-500, -500, -2000}, 1000)
scene_root:add_child(backwall)
backwall:set_material(white)
backwall:set_texture("Submission/class_backwall.png")



leftwall = gr.nh_box('leftwall', {-1500, -500, -1000}, 1000)
scene_root:add_child(leftwall)
leftwall:set_material(white)
leftwall:set_texture("Submission/class_leftwall.png")

rightwall = gr.nh_box('rightwall', {500, -500, -1000}, 1000)
scene_root:add_child(rightwall)
rightwall:set_material(white)
rightwall:set_texture("Submission/class_rightwall.png")

topwall = gr.nh_box('topwall', {-500, 500, -1000}, 1000)
scene_root:add_child(topwall)
topwall:set_material(white)
topwall:set_texture("Submission/class_leftwall.png")

bottomwall = gr.nh_box('bottomwall', {-500, -1500, -1000}, 1000)
scene_root:add_child(bottomwall)
bottomwall:set_material(white)
bottomwall:set_texture("Submission/class_floor.png")
-- bottomwall:set_texture("Submission/sample3.png")

-- table 1
cube1 = gr.cube('cube1')
scene_root:add_child(cube1)
cube1:set_material(table)
cube1:scale(80, 5, 80)
cube1:translate(-140, -90, 310)

cylinder1 = gr.cylinder('cylinder1')
scene_root:add_child(cylinder1)
cylinder1:set_material(blackish)
cylinder1:scale(4, 40, 4)
cylinder1:translate(-140, -130, 310)

cylinder2 = gr.cylinder('cylinder2')
scene_root:add_child(cylinder2)
cylinder2:set_material(blackish)
cylinder2:scale(4, 40, 4)
cylinder2:translate(-60, -130, 310)

cylinder3 = gr.cylinder('cylinder3')
scene_root:add_child(cylinder3)
cylinder3:set_material(blackish)
cylinder3:scale(4, 40, 4)
cylinder3:translate(-140, -130, 390)

cylinder4 = gr.cylinder('cylinder4')
scene_root:add_child(cylinder4)
cylinder4:set_material(blackish)
cylinder4:scale(4, 40, 4)
cylinder4:translate(-60, -130, 390)



-- table 2
cube2 = gr.cube('cube2')
scene_root:add_child(cube2)
cube2:set_material(table)
cube2:scale(80, 5, 80)
cube2:translate(60, -90, 310)

cylinder5 = gr.cylinder('cylinder5')
scene_root:add_child(cylinder5)
cylinder5:set_material(blackish)
cylinder5:scale(4, 40, 4)
cylinder5:translate(60, -130, 310)

cylinder6 = gr.cylinder('cylinder6')
scene_root:add_child(cylinder6)
cylinder6:set_material(blackish)
cylinder6:scale(4, 40, 4)
cylinder6:translate(140, -130, 310)

cylinder7 = gr.cylinder('cylinder7')
scene_root:add_child(cylinder7)
cylinder7:set_material(blackish)
cylinder7:scale(4, 40, 4)
cylinder7:translate(60, -130, 390)

cylinder8 = gr.cylinder('cylinder8')
scene_root:add_child(cylinder8)
cylinder8:set_material(blackish)
cylinder8:scale(4, 40, 4)
cylinder8:translate(140, -130, 390)


-- chair 1
cube3 = gr.cube('cube3')
scene_root:add_child(cube3)
cube3:set_material(chair_back)
cube3:scale(40, 5, 50)
cube3:translate(-110, -120, 390)

cylinder9 = gr.cylinder('cylinder9')
scene_root:add_child(cylinder9)
cylinder9:set_material(blackish)
cylinder9:scale(4, 20, 4)
cylinder9:translate(-110, -140, 400)

cylinder10 = gr.cylinder('cylinder10')
scene_root:add_child(cylinder10)
cylinder10:set_material(blackish)
cylinder10:scale(4, 20, 4)
cylinder10:translate(-70, -140, 400)

cylinder11 = gr.cylinder('cylinder11')
scene_root:add_child(cylinder11)
cylinder11:set_material(blackish)
cylinder11:scale(4, 20, 4)
cylinder11:translate(-110, -140, 425)

cylinder12 = gr.cylinder('cylinder12')
scene_root:add_child(cylinder12)
cylinder12:set_material(blackish)
cylinder12:scale(4, 20, 4)
cylinder12:translate(-70, -140, 425)

cube4 = gr.cube('cube4')
scene_root:add_child(cube4)
cube4:set_material(chair_back)
cube4:scale(40, 50, 5)
cube4:translate(-110, -120, 440)

-- chair 2
cube5 = gr.cube('cube5')
scene_root:add_child(cube5)
cube5:set_material(chair_back)
cube5:scale(40, 5, 50)
cube5:translate(70, -120, 390)

cylinder13 = gr.cylinder('cylinder13')
scene_root:add_child(cylinder13)
cylinder13:set_material(blackish)
cylinder13:scale(4, 20, 4)
cylinder13:translate(70, -140, 400)

cylinder14 = gr.cylinder('cylinder14')
scene_root:add_child(cylinder14)
cylinder14:set_material(blackish)
cylinder14:scale(4, 20, 4)
cylinder14:translate(110, -140, 400)

cylinder15 = gr.cylinder('cylinder15')
scene_root:add_child(cylinder15)
cylinder15:set_material(blackish)
cylinder15:scale(4, 20, 4)
cylinder15:translate(70, -140, 425)

cylinder16 = gr.cylinder('cylinder16')
scene_root:add_child(cylinder16)
cylinder16:set_material(blackish)
cylinder16:scale(4, 20, 4)
cylinder16:translate(110, -140, 425)

cube6 = gr.cube('cube6')
scene_root:add_child(cube6)
cube6:set_material(chair_back)
cube6:scale(40, 50, 5)
cube6:translate(70, -120, 440)


-- platform

platform = gr.cube('platform')
scene_root:add_child(platform)
platform:set_material(brown)
platform:set_texture("Submission/carpet.png")
platform:scale(1000, 60, 90)
platform:translate(-500, -500, -810)






white_light = gr.light({-100.0, 150.0, 400.0}, {0.9, 0.9, 0.9}, {1, 0, 0})
orange_light = gr.light({400.0, 100.0, 150.0}, {0.7, 0.0, 0.7}, {1, 0, 0})

my_light = gr.light({0, 100, 0}, {0.9, 0.9, 0.9}, {1, 0, 0})
my_light_2 = gr.light({-200, 200, 500}, {0.9, 0.9, 0.9}, {1, 0, 0})

al_wh = gr.area_light({-100.0, 150.0, 400.0}, {0.9, 0.9, 0.9}, {1, 0, 0}, 15)
al_or = gr.area_light({400.0, 100.0, 150.0}, {0.7, 0.0, 0.7}, {1, 0, 0}, 15)


gr.render(scene_root, 'final_scene.png', 512, 512,
	  {0, 0, 800}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.3, 0.3, 0.3}, {my_light})
