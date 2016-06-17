-- puppet.lua
-- A simplified puppet without posable joints, but that
-- looks roughly humanoid.

rootnode = gr.node('rootnode')

function puppet()

	suitRed = gr.material({0.945, 0.0, 0.0}, {0.2, 0.2, 0.2}, 10)
	blue = gr.material({0.0, 0.0, 1.0}, {0.1, 0.1, 0.1}, 10)
	black = gr.material({0.0, 0.0, 0.0}, {1.0, 1.0, 1.0}, 20)
	teethWhite = gr.material({1.0, 1.0, 1.0}, {0.1, 0.1, 0.1}, 100)
	white = gr.material({1.0, 1.0, 1.0}, {0.1, 0.1, 0.1}, 10)
	skin = gr.material({1.0, 0.7686, 0.5725}, {1.0, 1.0, 1.0}, 1000)
	noseSkin = gr.material({249/255, 80/255, 99/255}, {0.8, 0.8, 0.8}, 1000)
	goggles = gr.material({30/255, 36/255, 136/255}, {1.0, 1.0, 1.0}, 1)
	goggleLens = gr.material({62/255, 183/255, 55/255}, {0.9, 0.9, 0.9}, 500)
	hair = gr.material({210/255, 111/255, 10/255}, {0.1, 0.1, 0.1}, 1000)
	cuffYellow = gr.material({241/255, 171/255, 0}, {0.1, 0.1, 0.1}, 1000)
	suitYellow = gr.material({241/255, 210/255, 0}, {0.1, 0.1, 0.1}, 10)

	puppet = gr.node('puppet')

	torso = gr.mesh('sphere', 'torso')
	puppet:add_child(torso)
	torso:set_material(black)
	torso:scale(0.59,0.6,0.5);

	torsoPin1 = gr.mesh('sphere', 'torsoPin1')
	torso:add_child(torsoPin1)
	torsoPin1:set_material(goggles)
	torsoPin1:scale(0.08, 0.08, 0.08);
	torsoPin1:translate(-0.25, -0.25, 0.33)

	torsoPin2 = gr.mesh('sphere', 'torsoPin2')
	torso:add_child(torsoPin2)
	torsoPin2:set_material(goggles)
	torsoPin2:scale(0.08, 0.08, 0.08);
	torsoPin2:translate(0.25, -0.25, 0.33)

	torsoShirt = gr.mesh('sphere', 'torsoShirt')
	torso:add_child(torsoShirt)
	torsoShirt:set_material(suitRed)
	torsoShirt:scale(0.6, 0.62,0.51);
	torsoShirt:translate(0.0, 0.05, 0.0)

	shirtCube1 = gr.mesh('cube', 'shirtCube1')
	torsoShirt:add_child(shirtCube1)
	shirtCube1:set_material(suitYellow)
	shirtCube1:scale(0.1, 0.1,0.1);
	shirtCube1:translate(-0.15, 0.3, 0.435)

	shirtCube2 = gr.mesh('cube', 'shirtCube2')
	torsoShirt:add_child(shirtCube2)
	shirtCube2:set_material(suitYellow)
	shirtCube2:scale(0.1, 0.1,0.1);
	shirtCube2:translate(-0.15, 0.1, 0.45)

	shirtCube3 = gr.mesh('cube', 'shirtCube3')
	torsoShirt:add_child(shirtCube3)
	shirtCube3:set_material(suitYellow)
	shirtCube3:scale(0.1, 0.1,0.1);
	shirtCube3:translate(0.15, 0.3, 0.435)

	shirtCube4 = gr.mesh('cube', 'shirtCube4')
	torsoShirt:add_child(shirtCube4)
	shirtCube4:set_material(suitYellow)
	shirtCube4:scale(0.1, 0.1,0.1);
	shirtCube4:translate(0.15, 0.1, 0.45)

	-------- FACE ----------------------------------

	neck = gr.mesh('sphere', 'neck')
	torso:add_child(neck)
	neck:scale(0.15, 0.2, 0.15)
	neckJoint = gr.joint('neckJoint', {0,0,0}, {-90,0,90});
	neck:add_child(neckJoint);
	neck:translate(0.0, 0.6, 0.01)
	neck:set_material(skin)

	head = gr.mesh('sphere', 'head')
	neckJoint:add_child(head)
	head:scale(0.25, 0.25, 0.25)
	head:translate(0.0, 0.8, 0.0)
	head:set_material(skin)

	nose = gr.mesh('sphere', 'nose')
	head:add_child(nose)
	nose:scale(0.06, 0.06, 0.14)
	nose:rotate('x', 30)
	nose:translate(0.0, 0.78, 0.22)
	nose:set_material(noseSkin)

	leftEye = gr.mesh('cylinder', 'leftEye')
	head:add_child(leftEye)
	leftEye:scale(0.11, 0.15, 0.11)
	leftEye:rotate('x',90)
	leftEye:translate(-0.16, 0.89, 0.1)
	leftEye:set_material(goggles)

	leftLens = gr.mesh('cylinder', 'leftLens')
	leftEye:add_child(leftLens)
	leftLens:scale(0.09, 0.15, 0.09)
	leftLens:rotate('x',90)
	leftLens:translate(-0.154, 0.877, 0.11)
	leftLens:set_material(goggleLens)

	rightEye = gr.mesh('cylinder', 'rightEye')
	head:add_child(rightEye)
	rightEye:scale(0.11, 0.15, 0.11)
	rightEye:rotate('x', 90)
	rightEye:translate(0.059, 0.89, 0.1)
	rightEye:set_material(goggles)

	rightLens = gr.mesh('cylinder', 'rightLens')
	rightEye:add_child(rightLens)
	rightLens:scale(0.09, 0.15, 0.09)
	rightLens:rotate('x',90)
	rightLens:translate(0.068, 0.877, 0.11)
	rightLens:set_material(goggleLens)

	leftEyebrow = gr.mesh('sphere', 'leftEyebrow')
	head:add_child(leftEyebrow)
	leftEyebrow:scale(0.05, 0.02, 0.01)
	leftEyebrow:rotate('z',-45)
	leftEyebrow:translate(-0.08, 0.89, 0.24)
	leftEyebrow:set_material(hair)

	rightEyebrow = gr.mesh('sphere', 'rightEyebrow')
	head:add_child(rightEyebrow)
	rightEyebrow:scale(0.05, 0.02, 0.01)
	rightEyebrow:rotate('z',45)
	rightEyebrow:translate(0.084, 0.89, 0.24)
	rightEyebrow:set_material(hair)

	leftMoustache = gr.mesh('suzanne', 'leftMoustache')
	head:add_child(leftMoustache)
	leftMoustache:scale(0.2, 0.04, 0.02)
	leftMoustache:translate(-0.115, 0.735, 0.222)
	leftMoustache:set_material(hair)

	rightMoustache = gr.mesh('suzanne', 'rightMoustache')
	head:add_child(rightMoustache)
	rightMoustache:scale(0.2, 0.04, 0.02)
	rightMoustache:translate(0.115, 0.735, 0.222)
	rightMoustache:set_material(hair)

	mouth = gr.mesh('cube', 'mouth')
	head:add_child(mouth)
	mouth:scale(0.15, 0.04, 0.02)
	mouth:translate(0.0, 0.69, 0.21)
	mouth:set_material(teethWhite)

	---------- ARMS ------------------------------------------

	leftShoulderJoint = gr.joint('leftShoulderJoint', {0,0,0}, {-45,0,45});
	torso:add_child(leftShoulderJoint);

	leftShoulder = gr.mesh('sphere', 'leftShoulder')
	leftShoulderJoint:add_child(leftShoulder)
	leftShoulder:scale(0.12, 0.12, 0.12)
	leftUpperArmJoint = gr.joint('leftUpperArmJoint', {-30,0,30}, {0,0,0});
	leftShoulder:add_child(leftUpperArmJoint);
	leftShoulder:translate(-0.35, 0.55, 0.0)
	leftShoulder:set_material(suitRed)

	leftUpperArm = gr.mesh('sphere', 'leftUpperArm')
	leftUpperArmJoint:add_child(leftUpperArm)
	leftUpperArm:scale(0.4, 0.09, 0.09)
	leftUpperArm:rotate('z', 30)
	leftElbowJoint = gr.joint('leftElbowJoint', {-45,0,45}, {0,0,0});
	leftUpperArm:add_child(leftElbowJoint);
	leftUpperArm:translate(-0.65, 0.44, 0.0)
	leftUpperArm:set_material(suitRed)

	leftElbow = gr.mesh('sphere', 'leftElbow')
	leftElbowJoint:add_child(leftElbow)
	leftElbow:scale(0.08, 0.08, 0.08)
	leftForearmJoint = gr.joint('leftForearmJoint', {0,0,0}, {-60,0,60});
	leftElbow:add_child(leftForearmJoint);
	leftElbow:translate(-0.93, 0.285, 0.0)
	leftElbow:set_material(suitRed)

	leftForearm = gr.mesh('sphere', 'leftForearm')
	leftForearmJoint:add_child(leftForearm)
	leftForearm:scale(0.4, 0.11, 0.11)
	leftForearm:rotate('z', 90)
	leftForearm:translate(-0.94, -0.05, 0.0)
	leftForearm:set_material(suitRed)

	leftCuff= gr.mesh('cylinder', 'leftCuff')
	leftForearm:add_child(leftCuff)
	leftCuff:scale(0.2, 0.15, 0.2)
	leftWristJoint = gr.joint('leftWristJoint', {-60,0,90}, {0,60,0});
	leftCuff:add_child(leftWristJoint);
	leftCuff:translate(-1.04, -0.46, -0.11)
	leftCuff:set_material(cuffYellow)

	leftHand = gr.mesh('sphere', 'leftHand')
	leftWristJoint:add_child(leftHand)
	leftHand:rotate('y',90)
	leftHand:scale(0.12, 0.12, 0.12)
	leftHand:translate(-0.94, -0.55, 0.0)
	leftHand:set_material(white)

	finger1 = gr.mesh('sphere', 'finger1')
	leftHand:add_child(finger1)
	finger1:scale(0.2, 0.05, 0.05)
	finger1:rotate('z', -25)
	finger1:translate(-0.85, -0.55, 0.0)
	finger1:set_material(white)

	finger2 = gr.mesh('sphere', 'finger2')
	leftHand:add_child(finger2)
	finger2:scale(0.2, 0.05, 0.05)
	finger2:rotate('z', 75)
	finger2:translate(-1.04, -0.65, 0.0)
	finger2:set_material(white)

	finger3 = gr.mesh('sphere', 'finger3')
	leftHand:add_child(finger3)
	finger3:scale(0.2, 0.05, 0.05)
	finger3:rotate('z', 90)
	finger3:translate(-0.95, -0.7, 0.0)
	finger3:set_material(white)

	finger4 = gr.mesh('sphere', 'finger4')
	leftHand:add_child(finger4)
	finger4:scale(0.2, 0.05, 0.05)
	finger4:rotate('z', 105)
	finger4:translate(-0.85, -0.65, 0.0)
	finger4:set_material(white)

	leftHand:translate(0.94, 0.55, 0.0)
	leftHand:rotate('y',-90)
	leftHand:translate(-0.94, -0.55, 0.0)

	rightShoulderJoint = gr.joint('rightShoulderJoint', {0,0,0}, {-45,0,45});
	torso:add_child(rightShoulderJoint);

	rightShoulder = gr.mesh('sphere', 'rightShoulder')
	rightShoulderJoint:add_child(rightShoulder)
	rightShoulder:scale(0.12, 0.12, 0.12)
	rightUpperArmJoint = gr.joint('rightUpperArmJoint', {-30,0,30}, {0,0,0});
	rightShoulder:add_child(rightUpperArmJoint);
	rightShoulder:translate(0.35, 0.55, 0.0)
	rightShoulder:set_material(suitRed)

	rightUpperArm = gr.mesh('sphere', 'rightUpperArm')
	rightUpperArmJoint:add_child(rightUpperArm)
	rightUpperArm:scale(0.4, 0.09, 0.09)
	rightUpperArm:rotate('z', -30)
	rightElbowJoint = gr.joint('rightElbowJoint', {-45,0,45}, {0,0,0});
	rightUpperArm:add_child(rightElbowJoint);
	rightUpperArm:translate(0.65, 0.44, 0.0)
	rightUpperArm:set_material(suitRed)

	rightElbow = gr.mesh('sphere', 'rightElbow')
	rightElbowJoint:add_child(rightElbow)
	rightElbow:scale(0.08, 0.08, 0.08)
	rightForearmJoint = gr.joint('rightForearmJoint', {0,0,0}, {-60,0,60});
	rightElbow:add_child(rightForearmJoint);
	rightElbow:translate(0.93, 0.285, 0.0)
	rightElbow:set_material(suitRed)

	rightForearm = gr.mesh('sphere', 'rightForearm')
	rightForearmJoint:add_child(rightForearm)
	rightForearm:scale(0.4, 0.11, 0.11)
	rightForearm:rotate('z', 90)
	rightForearm:translate(0.94, -0.05, 0.0)
	rightForearm:set_material(suitRed)

	rightCuff= gr.mesh('cylinder', 'rightCuff')
	rightForearm:add_child(rightCuff)
	rightCuff:scale(0.2, 0.15, 0.2)
	rightWristJoint = gr.joint('rightWristJoint', {-60,0,90}, {0,0,0});
	rightCuff:add_child(rightWristJoint);
	rightCuff:translate(0.84, -0.46, -0.11)
	rightCuff:set_material(cuffYellow)

	rightHand = gr.mesh('sphere', 'rightHand')
	rightWristJoint:add_child(rightHand)
	rightHand:scale(0.12, 0.12, 0.12)
	rightHand:translate(0.94, -0.55, 0.0)
	rightHand:set_material(white)

	finger1 = gr.mesh('sphere', 'finger1')
	rightHand:add_child(finger1)
	finger1:scale(0.2, 0.05, 0.05)
	finger1:rotate('z', 25)
	finger1:translate(0.85, -0.55, 0.0)
	finger1:set_material(white)

	finger2 = gr.mesh('sphere', 'finger2')
	rightHand:add_child(finger2)
	finger2:scale(0.2, 0.05, 0.05)
	finger2:rotate('z', -75)
	finger2:translate(1.04, -0.65, 0.0)
	finger2:set_material(white)

	finger3 = gr.mesh('sphere', 'finger3')
	rightHand:add_child(finger3)
	finger3:scale(0.2, 0.05, 0.05)
	finger3:rotate('z', -90)
	finger3:translate(0.95, -0.7, 0.0)
	finger3:set_material(white)

	finger4 = gr.mesh('sphere', 'finger4')
	rightHand:add_child(finger4)
	finger4:scale(0.2, 0.05, 0.05)
	finger4:rotate('z', -105)
	finger4:translate(0.85, -0.65, 0.0)
	finger4:set_material(white)

	rightHand:translate(-0.94, 0.55, 0.0)
	rightHand:rotate('y',90)
	rightHand:translate(0.94, -0.55, 0.0)

	----------- LEGS ---------------------------------------------

	leftHipJoint = gr.joint('leftHipJoint', {0,0,0}, {-45,0,45});
	torso:add_child(leftHipJoint);

	leftHip = gr.mesh('sphere', 'leftHip')
	leftHipJoint:add_child(leftHip)
	leftHip:scale(0.15, 0.15, 0.15)
	leftThighJoint = gr.joint('leftThighJoint', {-30,0,30}, {0,0,0});
	leftHip:add_child(leftThighJoint);
	leftHip:translate(-0.28, -0.52, 0.0)
	leftHip:set_material(black)

	leftThigh = gr.mesh('sphere', 'leftThigh')
	leftThighJoint:add_child(leftThigh)
	leftThigh:scale(0.4, 0.11, 0.11)
	leftThigh:rotate('z', 70)
	leftKneeJoint = gr.joint('leftKneeJoint', {0,0,0}, {-30,0,30});
	leftThigh:add_child(leftKneeJoint);
	leftThigh:translate(-0.41, -0.83, 0.0)
	leftThigh:set_material(black)

	leftKnee = gr.mesh('sphere', 'leftKnee')
	leftKneeJoint:add_child(leftKnee)
	leftKnee:scale(0.09, 0.09, 0.09)
	leftCalfJoint = gr.joint('leftCalfJoint', {-60,0,60}, {0,0,0});
	leftKnee:add_child(leftCalfJoint);
	leftKnee:translate(-0.53, -1.13, 0.0)
	leftKnee:set_material(black)

	leftCalf = gr.mesh('sphere', 'leftCalf')
	leftCalfJoint:add_child(leftCalf)
	leftCalf:scale(0.4, 0.10, 0.10)
	leftCalf:rotate('z', 90)
	leftAnkleJoint = gr.joint('leftAnkleJoint', {0,0,0}, {-30,0,30});
	leftCalf:add_child(leftAnkleJoint);
	leftCalf:translate(-0.53, -1.43, 0.0)
	leftCalf:set_material(black)

	leftAnkleBall = gr.mesh('sphere', 'leftAnkleBall')
	leftCalf:add_child(leftAnkleBall)
	leftAnkleBall:scale(0.05, 0.05, 0.05)
	leftAnkleJoint = gr.joint('rightAnkleJoint', {0,0,0}, {-30,0,30});
	leftAnkleBall:add_child(leftAnkleJoint);
	leftAnkleBall:translate(-0.54, -1.75, 0.0)
	leftAnkleBall:set_material(black)

	leftFoot = gr.mesh('sphere', 'leftFoot')
	leftAnkleJoint:add_child(leftFoot)
	leftFoot:scale(0.3, 0.10, 0.10)
	leftFoot:rotate('y', 45)
	leftFoot:translate(-0.7, -1.76, 0.18)
	leftFoot:set_material(black)

	rightHipJoint = gr.joint('rightHipJoint', {0,0,0}, {-45,0,45});
	torso:add_child(rightHipJoint);

	rightHip = gr.mesh('sphere', 'rightHip')
	rightHipJoint:add_child(rightHip)
	rightHip:scale(0.15, 0.15, 0.15)
	rightThighJoint = gr.joint('rightThighJoint', {0,0,0}, {-30,0,30});
	rightHip:add_child(rightThighJoint);
	rightHip:translate(0.28, -0.52, 0.0)
	rightHip:set_material(black)

	rightThigh = gr.mesh('sphere', 'rightThigh')
	rightThighJoint:add_child(rightThigh)
	rightThigh:scale(0.4, 0.11, 0.11)
	rightThigh:rotate('z', -70)
	rightKneeJoint = gr.joint('rightKneeJoint', {0,0,0}, {-30,0,30});
	rightThigh:add_child(rightKneeJoint);
	rightThigh:translate(0.41, -0.83, 0.0)
	rightThigh:set_material(black)

	rightKnee = gr.mesh('sphere', 'rightKnee')
	rightKneeJoint:add_child(rightKnee)
	rightKnee:scale(0.09, 0.09, 0.09)
	rightCalfJoint = gr.joint('rightCalfJoint', {-60,0,60}, {0,0,0});
	rightKnee:add_child(rightCalfJoint);
	rightKnee:translate(0.53, -1.13, 0.0)
	rightKnee:set_material(black)

	rightCalf = gr.mesh('sphere', 'rightCalf')
	rightCalfJoint:add_child(rightCalf)
	rightCalf:scale(0.4, 0.10, 0.10)
	rightCalf:rotate('z', -90)
	rightCalf:translate(0.53, -1.43, 0.0)
	rightCalf:set_material(black)

	rightAnkleBall = gr.mesh('sphere', 'rightAnkleBall')
	rightCalf:add_child(rightAnkleBall)
	rightAnkleBall:scale(0.05, 0.05, 0.05)
	rightAnkleJoint = gr.joint('rightAnkleJoint', {0,0,0}, {-30,0,30});
	rightAnkleBall:add_child(rightAnkleJoint);
	rightAnkleBall:translate(0.54, -1.75, 0.0)
	rightAnkleBall:set_material(black)

	rightFoot = gr.mesh('sphere', 'rightFoot')
	rightAnkleJoint:add_child(rightFoot)
	rightFoot:scale(0.3, 0.10, 0.10)
	rightFoot:rotate('y', -45)
	rightFoot:translate(0.7, -1.76, 0.18)
	rightFoot:set_material(black)

	leftAnkleBall:translate(0.54, 1.75, 0.0)
	leftAnkleBall:rotate('y', -45)
	leftAnkleBall:translate(-0.54, -1.75, 0.0)

	leftHip:translate(0.28, 0.52, 0.0)
	leftHip:rotate('y',50)
	leftHip:translate(-0.28, -0.52, 0.0)

	rightAnkleBall:translate(-0.54, 1.75, 0.0)
	rightAnkleBall:rotate('y', 45)
	rightAnkleBall:translate(0.54, -1.75, 0.0)

	rightHip:translate(-0.28, 0.52, 0.0)
	rightHip:rotate('y',-50)
	rightHip:translate(0.28, -0.52, 0.0)

	puppet:rotate('y',180)
	puppet:scale( 0.2, 0.2, 0.2 )
	puppet:translate(0.0, 0.01, -1.0)
	
	return puppet
end

function house()

	House = gr.node('House')

	foundation = gr.mesh('cube', 'foundation')
	House:add_child(foundation)
	foundation:set_material(grey);

	roof = gr.mesh('cube', 'roof')
	House:add_child(roof)
	roof:set_material(brown);
	roof:scale(0.99,0.68,0.68)
	roof:rotate('x', 45)
	roof:translate(0,0.505,0)

	window1 = gr.mesh('cube', 'window1')
	House:add_child(window1)
	window1:set_material(glassblue)
	window1:scale(0.001,0.2,0.2)
	window1:translate(0.5,0.15,0.2)

	window2 = gr.mesh('cube', 'window2')
	House:add_child(window2)
	window2:set_material(glassblue)
	window2:scale(0.001,0.2,0.2)
	window2:translate(0.5,0.15,-0.2)

	door = gr.mesh('cube', 'door')
	House:add_child(door)
	door:set_material(doorRed)
	door:scale(0.001,0.6,0.25)
	door:translate(-0.5,-0.15,0.0)

	window3 = gr.mesh('cube', 'window3')
	House:add_child(window3)
	window3:set_material(glassblue)
	window3:scale(0.001,0.2,0.2)
	window3:translate(-0.5,0.35,0.0)

	return House;

end

function barn()

	Barn = gr.node('Barn')

	foundation = gr.mesh('cube', 'foundation')
	Barn:add_child(foundation)
	foundation:set_material(grey);

	roof = gr.mesh('cube', 'roof')
	Barn:add_child(roof)
	roof:set_material(brown);
	roof:scale(0.99,0.68,0.68)
	roof:rotate('x', 45)
	roof:translate(0,0.505,0)

	door = gr.mesh('cube', 'door')
	Barn:add_child(door)
	door:set_material(doorRed)
	door:scale(0.001,0.6,0.25)
	door:translate(-0.5,-0.15,0.0)

	return Barn;

end

function tree()

	Tree = gr.node('tree')

	tree1 = gr.mesh('tree', 'tree')
	Tree:add_child(tree1)
	tree1:set_material(treeGreen);

	tree2 = gr.mesh('tree', 'tree')
	Tree:add_child(tree2)
	tree2:set_material(treeGreen);
	tree2:rotate('y',90)

	tree3 = gr.mesh('tree', 'tree')
	Tree:add_child(tree3)
	tree3:set_material(treeGreen);
	tree3:rotate('y',180)

	tree4 = gr.mesh('tree', 'tree')
	Tree:add_child(tree4)
	tree4:set_material(treeGreen);
	tree4:rotate('y',270)

	return Tree;

end

function world()
	
	world = gr.node('world')

	cornstalkYellow = gr.material({255/255, 239/255, 88/255}, {0.2, 0.2, 0.2}, 0.5)
	orange = gr.material({204/255, 102/255, 0}, {0.2, 0.2, 0.2}, 0.5)
	treeGreen = gr.material({77/255, 158/255, 58/255}, {0.2, 0.2, 0.2}, 10)
	grey = gr.material({139/255, 136/255, 120/255}, {0.2, 0.2, 0.2}, 100)
	darkgrey = gr.material({84/255, 84/255, 84/255}, {0.2, 0.2, 0.2}, 100)
	grassGreen = gr.material({0.0, 1.0, 0.0}, {0.2, 0.2, 0.2}, 10)
	brown = gr.material({139/255, 69/255, 19/255}, {1.0, 1.0, 1.0}, 100)
	soilBrown = gr.material({133/255, 87/255, 35/255}, {1.0, 1.0, 1.0}, 10000)
	glassblue = gr.material({0.5290, 0.8078, 0.9804}, {1.0, 1.0, 1.0}, 10000)
	doorRed = gr.material({51/255, 25/255, 0}, {0.2, 0.2, 0.2}, 1)

	perimeterDimension = 100

	ground = gr.mesh('cube', 'ground')
	world:add_child(ground)
	ground:set_material(grassGreen)
	ground:scale(perimeterDimension, 0.01, perimeterDimension)
	ground:translate(0,-0.36, 0)

	wall1 = gr.mesh('cube', 'outerwall')
	world:add_child(wall1)
	wall1:set_material(darkgrey)
	wall1:scale(1,3,perimeterDimension)
	wall1:translate(-perimeterDimension/2,0,0)

	wall2 = gr.mesh('cube', 'outerwall')
	world:add_child(wall2)
	wall2:set_material(darkgrey)
	wall2:scale(1,3,perimeterDimension)
	wall2:translate(perimeterDimension/2,0,0)

	wall3 = gr.mesh('cube', 'outerwall')
	world:add_child(wall3)
	wall3:set_material(darkgrey)
	wall3:scale(perimeterDimension,3,1)
	wall3:translate(0,0,perimeterDimension/2)

	wall4 = gr.mesh('cube', 'outerwall')
	world:add_child(wall4)
	wall4:set_material(darkgrey)
	wall4:scale(perimeterDimension,3,1)
	wall4:translate(0,0,-perimeterDimension/2)
	
	building_1 = gr.node('building_1')

	building1 = gr.mesh('cube', 'building')
	building_1:add_child(building1)
	building1:set_material(grey);

	roof = gr.mesh('cube', 'roof')
	building_1:add_child(roof)
	roof:set_material(brown)
	roof:scale(0.99,0.5,0.5)
	roof:rotate('x',45)
	roof:translate(0,0.5,0)
	
	pillar1 = gr.mesh('cube', 'pillar')
	building_1:add_child(pillar1)
	pillar1:set_material(grey)
	pillar1:scale(0.08,0.3,0.12)
	pillar1:translate(0.46,0.65,0.44)
	
	pillar2 = gr.mesh('cube', 'pillar')
	building_1:add_child(pillar2)
	pillar2:set_material(grey)
	pillar2:scale(0.08,0.3,0.12)
	pillar2:translate(-0.46,0.65,-0.44)
	
	pillar3 = gr.mesh('cube', 'pillar')
	building_1:add_child(pillar3)
	pillar3:set_material(grey)
	pillar3:scale(0.08,0.3,0.12)
	pillar3:translate(-0.46,0.65,0.44)
	
	pillar4 = gr.mesh('cube', 'pillar')
	building_1:add_child(pillar4)
	pillar4:set_material(grey)
	pillar4:scale(0.08,0.3,0.12)
	pillar4:translate(0.46,0.65,-0.44)
	
	window1 = gr.mesh('cube', 'window1')
	building_1:add_child(window1)
	window1:set_material(glassblue)
	window1:scale(0.001,0.1,0.1)
	window1:translate(0.5,0.35,0.3)
	
	window2 = gr.mesh('cube', 'window2')
	building_1:add_child(window2)
	window2:set_material(glassblue)
	window2:scale(0.001,0.1,0.1)
	window2:translate(0.5,0.35,0.1)
	
	window3 = gr.mesh('cube', 'window3')
	building_1:add_child(window3)
	window3:set_material(glassblue)
	window3:scale(0.001,0.1,0.1)
	window3:translate(0.5,0.35,-0.1)
	
	window4 = gr.mesh('cube', 'window4')
	building_1:add_child(window4)
	window4:set_material(glassblue)
	window4:scale(0.001,0.1,0.1)
	window4:translate(0.5,0.35,-0.3)
	
	door = gr.mesh('cube', 'door')
	building_1:add_child(door)
	door:set_material(doorRed)
	door:scale(0.001,0.2,0.25)
	door:translate(0.5,0.05,0.0)

	world:add_child(building_1)
	
	castlewall1 = gr.mesh('cube', 'wall')
	world:add_child(castlewall1)
	castlewall1:set_material(grey)
	castlewall1:scale(0.5,2,2)
	castlewall1:translate(4.75,0,-12)
	
	castlewall2 = gr.mesh('cube', 'wall')
	world:add_child(castlewall2)
	castlewall2:set_material(grey)
	castlewall2:scale(3,2,0.2)
	castlewall2:translate(6.5,0,-11.1)
	
	castlewall3 = gr.mesh('cube', 'wall')
	world:add_child(castlewall3)
	castlewall3:set_material(grey)
	castlewall3:scale(0.5,2,3)
	castlewall3:translate(7.75,0,-12.5)
	
	castlewall4 = gr.mesh('cube', 'wall')
	world:add_child(castlewall4)
	castlewall4:set_material(grey)
	castlewall4:scale(0.5,2,2)
	castlewall4:translate(4.75,0,-18)
	
	castlewall5 = gr.mesh('cube', 'wall')
	world:add_child(castlewall5)
	castlewall5:set_material(grey)
	castlewall5:scale(3,2,0.2)
	castlewall5:translate(6.5,0,-18.9)
	
	castlewall6 = gr.mesh('cube', 'wall')
	world:add_child(castlewall6)
	castlewall6:set_material(grey)
	castlewall6:scale(0.5,2,3)
	castlewall6:translate(7.75,0,-17.4)
	
	archway = gr.mesh('cube', 'archway')
	castlewall6:add_child(archway)
	archway:set_material(orange)
	archway:scale(0.48,0.1,3)
	archway:translate(7.75,0.95,-14.6)
	
	archwaymarker1 = gr.mesh('sphere', 'archwaymarker1')
	archway:add_child(archwaymarker1)
	archwaymarker1:set_material(orange)
	archwaymarker1:scale(0.2,0.2,0.2)
	archwaymarker1:translate(7.75,1,-13.7)
	
	archwaymarker2 = gr.mesh('sphere', 'archwaymarker2')
	archway:add_child(archwaymarker2)
	archwaymarker2:set_material(orange)
	archwaymarker2:scale(0.2,0.2,0.2)
	archwaymarker2:translate(7.75,1,-16.2)

	building_1:scale(10,8,5)
	building_1:translate(0,0,-15)

	building_2 = gr.node('building_2')

	building1 = gr.mesh('cube', 'building')
	building_2:add_child(building1)
	building1:set_material(grey);

	roof = gr.mesh('cube', 'roof')
	building_2:add_child(roof)
	roof:set_material(brown)
	roof:scale(0.99,0.5,0.5)
	roof:rotate('x',45)
	roof:translate(0,0.5,0.14)

	building_2:scale(3,3,8)
	building_2:translate(0,0,-12)
	world:add_child(building_2)

	building3 = gr.mesh('cube', 'building')
	building3:scale(3,5,5)
	building3:translate(-5,0,-12)
	world:add_child(building3)
	building3:set_material(grey);
	
	cylinder = gr.mesh('cylinder', 'building')
	cylinder:scale(2,3,2)
	cylinder:translate(-6.5,2,-11.5)
	world:add_child(cylinder)
	cylinder:set_material(grey);
	
	building3 = gr.mesh('cube', 'building')
	building3:scale(10,3,3)
	building3:translate(-10,0,-11.01)
	world:add_child(building3)
	building3:set_material(grey);
	
	building3 = gr.mesh('cube', 'building')
	building3:scale(3,8,3)
	building3:translate(-12,0,-15)
	world:add_child(building3)
	building3:set_material(grey);
	
	cylinder = gr.mesh('cylinder', 'building')
	cylinder:scale(1,3,1)
	cylinder:translate(-11,0.5,-11.5)
	world:add_child(cylinder)
	cylinder:set_material(grey);
	
	cylinder = gr.mesh('cylinder', 'building')
	cylinder:scale(1,3,1)
	cylinder:translate(-15,0.5,-11.5)
	world:add_child(cylinder)
	cylinder:set_material(grey);
	
	building_2 = gr.node('building_2')

	building1 = gr.mesh('cube', 'building')
	building_2:add_child(building1)
	building1:set_material(grey);

	roof = gr.mesh('cube', 'roof')
	building_2:add_child(roof)
	roof:set_material(brown)
	roof:scale(0.99,0.5,0.5)
	roof:rotate('x',45)
	roof:translate(0,0.5,0.14)

	building_2:scale(3,6,3)
	building_2:translate(-12,0,-15)
	world:add_child(building_2)
	
	building3 = gr.mesh('cube', 'building')
	building3:scale(10,0.8,0.8)
	building3:translate(-8,3.5,-16)
	world:add_child(building3)
	building3:set_material(grey);

	movingDoor = gr.mesh('cube', 'movingDoor')
	world:add_child(movingDoor)
	movingDoor:set_material(doorRed)
	movingDoor:scale(0.3,1.5,2.5)
	movingDoor:translate(7.7,0,-15)

	tree1 = tree();
	world:add_child(tree1);
	tree1:translate(0,0,-5)
	
	tree2 = tree()
	world:add_child(tree2);
	tree2:translate(10,0,-18)
	
	tree4 = tree()
	world:add_child(tree4);
	tree4:translate(6.3,0,-12.3)
	
	tree5 = tree();
	world:add_child(tree5);
	tree5:translate(3,0,-6)
	
	tree6 = tree()
	world:add_child(tree6);
	tree6:translate(25,0,18)

	tree7 = tree()
	world:add_child(tree7);
	tree7:translate(18.5,0,-15.5)

	house1 = house()
	world:add_child(house1)
	house1:scale(2,2,2)
	house1:rotate('y',90)
	house1:translate(15,0,-15)

	house2 = house()
	world:add_child(house2)
	house2:scale(2,2,2)
	house2:rotate('y',-90)
	house2:translate(17,0,-7)

	house3 = house()
	world:add_child(house3)
	house3:scale(2,2,2)
	house3:rotate('y',-90)
	house3:translate(20,0,-7)

	house4 = house()
	world:add_child(house4)
	house4:scale(2,2,2)
	house4:rotate('y',45)
	house4:translate(22.5,0,-17)

	house5 = house()
	world:add_child(house5)
	house5:scale(2,2,2)
	house5:rotate('y',-135)
	house5:translate(12,0,-8)

	house6 = house()
	world:add_child(house6)
	house6:scale(3,3,3)
	house6:translate(26,0,-12)

	house7 = house()
	world:add_child(house7)
	house7:scale(2,2,2)
	house7:rotate('y',180)
	house7:translate(6,0,-8)

	for i=1,4 do
		for j=1,5 do
			forestTree = tree()
			forestTree:scale(0.5*i,0.5*i,0.5*i)
			world:add_child(forestTree);
			forestTree:translate(-10-3*i*(i-1),0,-2*j*(j-1))
		end
	end

	for i=1,4 do
		for j=1,3 do
			forestTree = tree()
			forestTree:scale(1/j,1/j,1/j)
			world:add_child(forestTree);
			forestTree:translate(12+3*i*(i-1),0,-2+j*(j-1))
		end
	end

	for i=1,5 do
		for j=1,5 do
			forestTree = tree()
			forestTree:scale(1/2,1/2,1/2)
			world:add_child(forestTree);
			forestTree:translate(-8+0.82*i*i+0.32*i,0,-4+0.94*j*j+0.6*j);
		end
	end
	
	soil = gr.mesh('cube', 'soil')
	ground:add_child(soil)
	soil:set_material(soilBrown)
	soil:scale(40,0.05,20)
	soil:translate(-11,-0.37,38)

	for i=1,20 do
		for j=1,10 do
			corn = gr.mesh('corn', 'barrel')
			corn:set_material(cornstalkYellow)
			corn:scale(0.25,0.25,0.25)
			ground:add_child(corn);
			corn:translate(10-2*i,-0.4,27+2*j)
		end
	end

	house8 = house()
	world:add_child(house8)
	house8:scale(2,2,2)
	house8:rotate('y',-90)
	house8:translate(-40,0,40)
	
	barn1 = barn()
	world:add_child(barn1)
	barn1:scale(8,2.8,2.2)
	barn1:rotate('y',-90)
	barn1:translate(-35,0,40)
	
	barnTree = tree()
	world:add_child(barnTree);
	barnTree:translate(-41,0,35)

	hugeTree = tree()
	world:add_child(hugeTree);
	hugeTree:scale(4,4,4);
	hugeTree:translate(30,0,35)

	hugeTreeHouse = house()
	world:add_child(hugeTreeHouse)
	hugeTreeHouse:scale(2,2,2)
	hugeTreeHouse:translate(35,0,30)

	bumpcube = gr.mesh('cube', 'bumpcube')
	world:add_child(bumpcube)
	bumpcube:set_material(grey);
	bumpcube:scale(2,2,2)
	bumpcube:translate(-40,0,-10)
	
	silo = gr.mesh('cylinder', 'building')
	world:add_child(silo)
	silo:set_material(grey);
	silo:scale(1.3,4,1.3)
	silo:translate(-36,-0.5,43)
	
	silo = gr.mesh('cylinder', 'building')
	world:add_child(silo)
	silo:set_material(grey);
	silo:scale(2,5,2)
	silo:translate(-1,5,-16)
	
	pavement = gr.mesh('cube', 'pavement')
	ground:add_child(pavement)
	pavement:set_material(darkgrey)
	pavement:scale(18,0.05,1)
	pavement:rotate('y',-25)
	pavement:translate(16,-0.379,-11.2)
	
	for i=1,4 do
		for j=1,4 do
			villagehouse = house()
			world:add_child(villagehouse)
			villagehouse:scale(2,2,2)
			villagehouse:rotate('y',180*i)
			villagehouse:translate(-4+4*i,0,-20-2.5*j)
		end
	end
	
	for i=1,6 do
		for j=1,6 do
			smallforesttree = tree()
			world:add_child(smallforesttree);
			smallforesttree:translate(13+1.99*i+0.77*i*i,0,-20-1.99*j-0.14*j*j)
		end
	end
	
	return world
end


puppet = puppet()
world = world()
rootnode:add_child(puppet)
rootnode:add_child(world)

return rootnode






