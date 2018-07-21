
red_wall = gr.material( {0.44, 0.137, 0.1886}, {0, 0, 0}, 25, {0,0,0,0}, 0.0)
green_wall = gr.material( {0.1386, 0.1327, 0.45}, {0, 0, 0}, 25,{0,0,0,0}, 0.0)
grey_wall = gr.material( {0.4649, 0.4823, 0.5002}, {0, 0, 0}, 25,{0,0,0,0}, 0.0)

mat3  = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 25,{0,0,0,0}, 0.0)	
mat4  = gr.material({0.7, 0.6, 1.0}, {0.5, 0.4, 0.8}, 25,{0,0,0,0}, 0.0)			


mirror1 = gr.material( {0.0, 0.0, 0.0},   {1.0, 1.0, 1.0},	25,{1,0.1,0,0}, 0.0)
mirror2 = gr.material( {0.0, 0.0, 0.0},   {1.0, 1.0, 1.0},	25,{1,1,0,0}, 0.0)
mirror3 = gr.material( {0.0, 0.0, 0.0},   {1.0, 1.0, 1.0},	25,{1,10,0,0}, 0.0)
mirror4 = gr.material( {0.0, 0.0, 0.0},   {1.0, 1.0, 1.0},	25,{1,100,0,0}, 0.0)
mirror5 = gr.material( {0.0, 0.0, 0.0},   {1.0, 1.0, 1.0},	25,{1,1000,0,0}, 0.0)
mirror6 = gr.material( {0.0, 0.0, 0.0},   {1.0, 1.0, 1.0},	25,{1,10000,0,0}, 0.0)

-- glass = gr.material( {0.05, 0.05, 0.05}, {0.7, 0.7, 0.7}, 25, {0,0,0,0}, 0.0)			
-- glass2 = gr.material( {0.05, 0.05, 0.05}, {0.7, 0.7, 0.7}, 25, {1,1000,0,0}, 0.0)			
-- glass3 = gr.material( {0.05, 0.05, 0.05}, {0.7, 0.7, 0.7}, 25, {1,10,0,0}, 0.0)	
-- glass = gr.material	({0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, 25,{0,0,0,0}, 1.5)
glass1 = gr.material( {0.05, 0.05, 0.05}, {0.7, 0.7, 0.7}, 25, {0,0,1,0.01}, 1.5)	
glass2 = gr.material( {0.05, 0.05, 0.05}, {0.7, 0.7, 0.7}, 25, {0,0,1,0.1}, 1.5)	
glass3 = gr.material( {0.05, 0.05, 0.05}, {0.7, 0.7, 0.7}, 25, {0,0,1,1}, 1.5)	
glass4 = gr.material( {0.05, 0.05, 0.05}, {0.7, 0.7, 0.7}, 25, {0,0,1,10}, 1.5)	
glass5 = gr.material( {0.05, 0.05, 0.05}, {0.7, 0.7, 0.7}, 25, {0,0,1,100}, 1.5)	
glass6 = gr.material( {0.05, 0.05, 0.05}, {0.7, 0.7, 0.7}, 25, {0,0,1,1000}, 1.5)	

glass = gr.material( {0.05, 0.05, 0.05}, {0.7, 0.7, 0.7}, 25, {0,0,0,0}, 1.5)	


scene = gr.node('scene')


floor = gr.mesh( 'floor', 'plane.obj' )
scene:add_child(floor)
floor:set_material(grey_wall)
floor:scale(30, 30, 30)
floor:translate(0, -10, 0)


roof = gr.mesh( 'roof', 'plane.obj' )
scene:add_child(roof)
roof:set_material(grey_wall)
roof:rotate('X', 180)
roof:scale(30, 30, 30)
roof:translate(0, 10, 0)


left_wall = gr.mesh( 'left_wall', 'plane.obj' )
scene:add_child(left_wall)
left_wall:set_material(red_wall)
left_wall:rotate('Z', -90)
left_wall:scale(30, 30, 30)
left_wall:translate(-10, 0, 0)


right_wall = gr.mesh( 'right_wall', 'plane.obj' )
scene:add_child(right_wall)
right_wall:set_material(green_wall)
right_wall:rotate('Z', 90)
right_wall:scale(30, 30, 30)
right_wall:translate(10, 0, 0)


back_wall = gr.mesh( 'back_wall', 'plane.obj' )
scene:add_child(back_wall)
back_wall:set_material(grey_wall)
back_wall:rotate('X', 90)
back_wall:scale(30, 30, 30)
back_wall:translate(0, 0, -10)

-- mirror_sphere0 = gr.sphere('mirror_sphere0')
-- scene:add_child(mirror_sphere0)
-- mirror_sphere0:set_material(mirror1)
-- -- mirror_sphere:set_glossy({1.0, 10, 0.0, 100})
-- mirror_sphere0:scale(2, 2, 2)
-- mirror_sphere0:translate(-6, 5, -3)

mirror_sphere1 = gr.sphere('mirror_sphere1')
scene:add_child(mirror_sphere1)
mirror_sphere1:set_material(glass6)
mirror_sphere1:scale(2, 2, 2)
mirror_sphere1:translate(1, -4, 4)


b1 = gr.cube('b1')
scene:add_child(b1)
b1:set_material(mat4)
b1:scale(3, 3, 3)
b1:translate(-5, -10, 1)

s1 = gr.sphere('s1')
scene:add_child(s1)
s1:set_material(mat3)
-- s1:scale(2, 2, 2)
s1:translate(-1, -6, -8)



-- mirror_sphere2 = gr.sphere('mirror_sphere2')
-- scene:add_child(mirror_sphere2)
-- mirror_sphere2:set_material(glass2)
-- mirror_sphere2:scale(2, 2, 2)
-- mirror_sphere2:translate(0, 4, 4)

-- mirror_sphere3 = gr.sphere('mirror_sphere3')
-- scene:add_child(mirror_sphere3)
-- mirror_sphere3:set_material(glass3)
-- mirror_sphere3:scale(2, 2, 2)
-- mirror_sphere3:translate(5, 4, 4)


-- mirror_sphere4 = gr.sphere('mirror_sphere4')
-- scene:add_child(mirror_sphere4)
-- mirror_sphere4:set_material(glass4)
-- mirror_sphere4:scale(2, 2, 2)
-- mirror_sphere4:translate(-5, -4, 4)

-- mirror_sphere5 = gr.sphere('mirror_sphere5')
-- scene:add_child(mirror_sphere5)
-- mirror_sphere5:set_material(glass5)
-- mirror_sphere5:scale(2, 2, 2)
-- mirror_sphere5:translate(0, -4, 4)

-- mirror_sphere6 = gr.sphere('mirror_sphere6')
-- scene:add_child(mirror_sphere6)
-- mirror_sphere6:set_material(glass6)
-- mirror_sphere6:scale(2, 2, 2)
-- mirror_sphere6:translate(5, -4, 4)


white_area_light = gr.area_light({2, 2, 2}, {0.8, 0.8, 0.8}, {1, 0, 0}, 0.5)
white_light = gr.light({2.0, 2.0, 4}, {0.8, 0.8, 0.8}, {1, 0, 0})


gr.render(scene, 'glossy_refraction_area_light.png', 1024, 1024,
	  {0, 0, 30}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.4, 0.4, 0.4}, {white_area_light})
