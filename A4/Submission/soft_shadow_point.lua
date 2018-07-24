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

magenta = gr.material( {0.835, 0.101, 0.698}, {0, 0, 0}, 25,{0,0,0,0}, 0.0)
mat3  = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 25,{0,0,0,0}, 0.0)	
mat4  = gr.material({0.7, 0.6, 1.0}, {0.5, 0.4, 0.8}, 25,{0,0,0,0}, 0.0)		

-- mirror = gr.materimat4  = gr.material({0.7, 0.6, 1.0}, {0.5, 0.4, 0.8}, 25,{0,0,0,0}, 0.0)		al( {0.0, 0.0, 0.0},   {1.0, 1.0, 1.0},	25,{0,0,0,0}, 0.0)

glass = gr.material( {0.05, 0.05, 0.05}, {0.7, 0.7, 0.7}, 25, {0,0,0,0}, 1.5)	

scene_root = gr.node('root')

sphere = gr.sphere('sphere')
scene_root:add_child(sphere)
sphere:set_material(light_wall)
sphere:scale(60, 60, 60)
sphere:translate(-80, -160, -200)

cube = gr.cube('cube')
scene_root:add_child(cube)
cube:set_material(brown)
cube:scale(80, 80, 80)
cube:translate(-80, -160, 0)

cone = gr.cone('cone')
scene_root:add_child(cone)
cone:set_material(red)
cone:scale(60, 120, 60)
cone:translate(80, -160, -200)

cylinder = gr.cylinder('cylinder')
scene_root:add_child(cylinder)
cylinder:set_material(magenta)
cylinder:scale(60, 50, 60)
cylinder:translate(80, -160, 0)


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

my_light = gr.light({0, 400, 400}, {0.9, 0.9, 0.9}, {1, 0, 0})


al_wh = gr.area_light({0, 400, 400}, {0.9, 0.9, 0.9}, {1, 0, 0}, 15)
al_or = gr.area_light({400.0, 100.0, 150.0}, {0.7, 0.0, 0.7}, {1, 0, 0}, 15)

my_al_wh = gr.area_light({0, 400, 400}, {0.9, 0.9, 0.9}, {1, 0, 0}, 15)


gr.render(scene_root, 'soft_shadow_point.png', 512, 512,
	  {0, 0, 800}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.3, 0.3, 0.3}, {my_light})
