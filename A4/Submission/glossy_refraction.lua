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
purple = gr.material( {0.337, 0.094, 0.764}, {0, 0, 0}, 25,{0,0,0,0}, 0.0)

magenta = gr.material( {0.835, 0.101, 0.698}, {0, 0, 0}, 25,{0,0,0,0}, 0.0)
mat3  = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 25,{0,0,0,0}, 0.0)	
mat4  = gr.material({0.7, 0.6, 1.0}, {0.5, 0.4, 0.8}, 25,{0,0,0,0}, 0.0)		


mirror = gr.material( {0.0, 0.0, 0.0},   {1.0, 1.0, 1.0},	25,{0,0,0,0}, 0.0)

glass = gr.material( {0.05, 0.05, 0.05}, {0.7, 0.7, 0.7}, 25, {0,0,0,0}, 1.5)
glass1 = gr.material( {0.05, 0.05, 0.05}, {0.7, 0.7, 0.7}, 25, {0,0,1,0.01}, 1.5)	
glass2 = gr.material( {0.05, 0.05, 0.05}, {0.7, 0.7, 0.7}, 25, {0,0,1,0.1}, 1.5)	
glass3 = gr.material( {0.05, 0.05, 0.05}, {0.7, 0.7, 0.7}, 25, {0,0,1,1}, 1.5)	
glass4 = gr.material( {0.05, 0.05, 0.05}, {0.7, 0.7, 0.7}, 25, {0,0,1,10}, 1.5)	
glass5 = gr.material( {0.05, 0.05, 0.05}, {0.7, 0.7, 0.7}, 25, {0,0,1,100}, 1.5)	
glass6 = gr.material( {0.05, 0.05, 0.05}, {0.7, 0.7, 0.7}, 25, {0,0,1,500}, 1.5)

scene_root = gr.node('root')

sphere1 = gr.sphere('sphere1')
scene_root:add_child(sphere1)
sphere1:set_material(glass6)
sphere1:scale(80, 80, 80)
sphere1:translate(0, 0, 180)

cube = gr.cube('cube')
scene_root:add_child(cube)
cube:set_material(purple)
cube:scale(100, 100, 100)
cube:translate(-130, -130, -400)



--sphere2 = gr.sphere('sphere2')
--scene_root:add_child(sphere2)
--sphere2:set_material(glass)
--sphere2:scale(90, 90, 90)
--sphere2:translate(-100, 100, 0)

--sphere3 = gr.sphere('sphere3')
--scene_root:add_child(sphere3)
--sphere3:set_material(glass)
--sphere3:scale(90, 90, 90)
--sphere3:translate(100, -100, 0)

--sphere4 = gr.sphere('sphere4')
--scene_root:add_child(sphere4)
--sphere4:set_material(glass)
--sphere4:scale(90, 90, 90)
--sphere4:translate(100, 100, 0)



--cylinder1 = gr.cylinder('cylinder1')
--scene_root:add_child(cylinder1)
--cylinder1:set_material(mirror)
--cylinder1:scale(80, 80, 80)
--cylinder1:translate(0, -200, -600)

--cylinder2 = gr.cylinder('cylinder2')
--scene_root:add_child(cylinder2)
--cylinder2:set_material(mirror)
--cylinder2:scale(80, 80, 80)
--cylinder2:translate(200, 0, -600)

--cylinder3 = gr.cylinder('cylinder3')
--scene_root:add_child(cylinder3)
--cylinder3:set_material(mirror)
--cylinder3:scale(80, 80, 80)
--cylinder3:translate(-200, 0, -600)

--cylinder4 = gr.cylinder('cylinder4')
--scene_root:add_child(cylinder4)
--cylinder4:set_material(mirror)
--cylinder4:scale(80, 80, 80)
--cylinder4:translate(0, 200, -600)



--cube = gr.cube('cube')
--scene_root:add_child(cube)
--cube:set_material(mirror)
--cube:scale(80, 80, 80)
--cube:translate(-140, 60, 60)

--cone = gr.cone('cone')
--scene_root:add_child(cone)
--cone:set_material(mirror)
--cone:scale(60, 120, 60)
--cone:translate(100, -160, 100)

--cylinder = gr.cylinder('cylinder')
--scene_root:add_child(cylinder)
--cylinder:set_material(mirror)
--cylinder:scale(60, 50, 60)
--cylinder:translate(100, 100, 100)


-- b1 = gr.cube('b1')
-- scene_root:add_child(b1)
-- b1:set_material(mat4)
-- b1:scale(3, 3, 3)
-- b1:translate(-5, -10, 1)

-- s1 = gr.sphere('s1')
-- scene_root:add_child(s1)
-- s1:set_material(mat3)
-- -- s1:scale(2, 2, 2)
-- s1:translate(-1, -6, -8)

backwall = gr.nh_box('backwall', {-500, -500, -2000}, 1000)
scene_root:add_child(backwall)
backwall:set_material(red_wall)
-- backwall:set_texture("Assets/sample3.png")



leftwall = gr.nh_box('leftwall', {-1500, -500, -1000}, 1000)
scene_root:add_child(leftwall)
leftwall:set_material(green_wall)

rightwall = gr.nh_box('rightwall', {500, -500, -1000}, 1000)
scene_root:add_child(rightwall)
rightwall:set_material(grey_wall)

topwall = gr.nh_box('topwall', {-500, 500, -1000}, 1000)
scene_root:add_child(topwall)
topwall:set_material(light_wall)

bottomwall = gr.nh_box('bottomwall', {-500, -1500, -1000}, 1000)
scene_root:add_child(bottomwall)
bottomwall:set_material(light_wall)
-- bottomwall:set_texture("Assets/sample3.png")



white_light = gr.light({-100.0, 150.0, 400.0}, {0.9, 0.9, 0.9}, {1, 0, 0})
orange_light = gr.light({400.0, 100.0, 150.0}, {0.7, 0.0, 0.7}, {1, 0, 0})

my_light = gr.light({100, 0, 800}, {0.9, 0.9, 0.9}, {1, 0, 0})
my_light_2 = gr.light({-200, 200, 500}, {0.7, 0.0, 0.7}, {1, 0, 0})


al_wh = gr.area_light({-100.0, 150.0, 400.0}, {0.9, 0.9, 0.9}, {1, 0, 0}, 15)
al_or = gr.area_light({400.0, 100.0, 150.0}, {0.7, 0.0, 0.7}, {1, 0, 0}, 15)


gr.render(scene_root, 'glossy_refraction.png', 512, 512,
	  {0, 0, 800}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.3, 0.3, 0.3}, {my_light, my_light_2})
