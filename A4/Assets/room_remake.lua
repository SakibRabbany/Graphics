-- A simple scene with some miscellaneous geometry.

-- mat1 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 25, {0, 1000, 0, 0}, 0.27)
-- mat2 = gr.material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, 25, {0, 0, 0, 0}, 0.05)
-- mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 25, {0, 0, 0, 0}, 2.41)
-- mat4 = gr.material({0.7, 0.6, 1.0}, {0.5, 0.4, 0.8}, 25, {0, 0, 0, 0}, 0.27)
-- mat5 = gr.material({0.7, 0.6, 1.0}, {0.5, 0.4, 0.8}, 25, {0, 1000, 0, 0}, 0.27)

red_wall = gr.material( {0.44, 0.137, 0.1886}, {0, 0, 0}, 25, {0,0,0,0}, 0.0)
green_wall = gr.material( {0.1386, 0.1327, 0.45}, {0, 0, 0}, 25,{0,0,0,0}, 0.0)
grey_wall = gr.material( {0.4649, 0.4823, 0.5002}, {0, 0, 0}, 25,{0,0,0,0}, 0.0)
rand_wall = gr.material( {0.1274, 0.8756, 0.5463}, {0, 0, 0}, 25,{0,0,0,0}, 0.0)

-- mirror = gr.material( {0.0, 0.0, 0.0},   {1.0, 1.0, 1.0},	25,{0,0,0,0}, 0.0)

glass = gr.material( {0.05, 0.05, 0.05}, {0.7, 0.7, 0.7}, 25, {0,0,0,0}, 1.5)	

scene_root = gr.node('root')

mirror_sphere1 = gr.sphere('mirror_sphere1')
scene_root:add_child(mirror_sphere1)
mirror_sphere1:set_material(glass)
mirror_sphere1:scale(120, 120, 120)
mirror_sphere1:translate(0, 0, 400)


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
topwall:set_material(rand_wall)

bottomwall = gr.nh_box('bottomwall', {-500, -1500, -1000}, 1000)
scene_root:add_child(bottomwall)
bottomwall:set_material(rand_wall)
-- bottomwall:set_texture("Assets/sample3.png")



white_light = gr.light({-100.0, 150.0, 400.0}, {0.9, 0.9, 0.9}, {1, 0, 0})
orange_light = gr.light({400.0, 100.0, 150.0}, {0.7, 0.0, 0.7}, {1, 0, 0})

my_light = gr.light({100, 0, 800}, {0.9, 0.9, 0.9}, {1, 0, 0})


al_wh = gr.area_light({-100.0, 150.0, 400.0}, {0.9, 0.9, 0.9}, {1, 0, 0}, 15)
al_or = gr.area_light({400.0, 100.0, 150.0}, {0.7, 0.0, 0.7}, {1, 0, 0}, 15)


gr.render(scene_root, 'remake.png', 256, 256,
	  {0, 0, 800}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.3, 0.3, 0.3}, {my_light})
