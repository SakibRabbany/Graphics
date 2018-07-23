-- A simple scene with some miscellaneous geometry.


mat1 = gr.material({0.7, 1.0, 0.7}, {0, 0, 0}, 25, {0,0,0,0}, 0)
mat2 = gr.material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, 25, {0,0,0,0}, 0)
mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 25, {0,0,0,0}, 0)
mat4 = gr.material({0.7, 0.6, 1.0}, {0.5, 0.4, 0.8}, 25, {0,0,0,0}, 0)

scene_root = gr.node('root')

s1 = gr.nh_sphere('s1', {320, 0, 0}, 40.5)
scene_root:add_child(s1)
s1:set_material(mat1)
s1:set_texture("Assets/moon.png")

s2 = gr.nh_sphere('s2', {0, 0, 100}, 150)
scene_root:add_child(s2)
s2:set_material(mat1)
s2:set_texture("Assets/earth.png")



-- A small stellated dodecahedron.

steldodec = gr.mesh( 'dodec', 'smstdodeca.obj' )
steldodec:set_material(mat3)
scene_root:add_child(steldodec)
steldodec:scale(60,60,60)
steldodec:translate(-2000, 10000, -50000)


white_light = gr.light({-100.0, 150.0, 400.0}, {0.9, 0.9, 0.9}, {1, 0, 0})
orange_light = gr.light({400.0, 100.0, 150.0}, {0.7, 0.0, 0.7}, {1, 0, 0})

gr.render(scene_root, 'earth_sun_moon.png', 500, 500,
	  {0, 0, 1000}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.3, 0.3, 0.3}, {white_light})
