within ;
package cranes
  model crane
    inner Modelica.Mechanics.MultiBody.World world
      annotation (Placement(transformation(extent={{-80,40},{-60,60}})));
    Modelica.Mechanics.MultiBody.Parts.BodyShape bodyShape(m=1,
      r_CM={0,0,0},
      r={0,0,0})
      annotation (Placement(transformation(extent={{18,40},{38,60}})));
    Modelica.Mechanics.MultiBody.Joints.Prismatic prismatic(useAxisFlange=true,
      v(fixed=true, start=0),
      s(fixed=false, start=0))
      annotation (Placement(transformation(extent={{-30,40},{-10,60}})));
    Modelica.Mechanics.Translational.Sources.Position position(useSupport=true)
      annotation (Placement(transformation(extent={{-34,68},{-14,88}})));
    Modelica.Blocks.Sources.Sine sine(amplitude=1, freqHz=0.2)
      annotation (Placement(transformation(extent={{-94,68},{-74,88}})));
    Modelica.Mechanics.MultiBody.Joints.Revolute revolute(cylinderLength=0.2,
      phi(fixed=true, start=0),
      w(fixed=true, start=0))
      annotation (Placement(transformation(
          extent={{-10,-10},{10,10}},
          rotation=270,
          origin={42,26})));
    Modelica.Mechanics.MultiBody.Parts.FixedTranslation fixedTranslation(r={0,-1,
          0}) annotation (Placement(transformation(
          extent={{-10,-10},{10,10}},
          rotation=270,
          origin={42,-10})));
    Modelica.Mechanics.MultiBody.Parts.Body body(m=500, r_CM={0,0,0})
                                                        annotation (Placement(
          transformation(
          extent={{-10,-11},{10,11}},
          rotation=270,
          origin={42,-39})));
  equation
    connect(world.frame_b, prismatic.frame_a) annotation (Line(
        points={{-60,50},{-30,50}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(bodyShape.frame_a, prismatic.frame_b) annotation (Line(
        points={{18,50},{-10,50}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(prismatic.support, position.support) annotation (Line(
        points={{-24,56},{-24,68}},
        color={0,127,0},
        smooth=Smooth.None));
    connect(position.flange, prismatic.axis) annotation (Line(
        points={{-14,78},{-14,56},{-12,56}},
        color={0,127,0},
        smooth=Smooth.None));
    connect(position.s_ref, sine.y) annotation (Line(
        points={{-36,78},{-73,78}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(bodyShape.frame_b, revolute.frame_a) annotation (Line(
        points={{38,50},{42,50},{42,36}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(revolute.frame_b, fixedTranslation.frame_a) annotation (Line(
        points={{42,16},{42,0}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(fixedTranslation.frame_b, body.frame_a) annotation (Line(
        points={{42,-20},{42,-29}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    annotation (                                 Diagram(coordinateSystem(
            preserveAspectRatio=false, extent={{-100,-100},{100,100}}), graphics));
  end crane;

  model crane_input
    inner Modelica.Mechanics.MultiBody.World world
      annotation (Placement(transformation(extent={{-70,50},{-50,70}})));
    Modelica.Mechanics.MultiBody.Parts.BodyShape bodyShape(m=1, r={0,0,0})
      annotation (Placement(transformation(extent={{28,50},{48,70}})));
    Modelica.Mechanics.MultiBody.Joints.Prismatic prismatic(useAxisFlange=true)
      annotation (Placement(transformation(extent={{-20,50},{0,70}})));
    Modelica.Mechanics.Translational.Sources.Position position(useSupport=true)
      annotation (Placement(transformation(extent={{-24,78},{-4,98}})));
    Modelica.Mechanics.MultiBody.Joints.Revolute revolute(cylinderLength=0.2)
      annotation (Placement(transformation(
          extent={{-10,-10},{10,10}},
          rotation=270,
          origin={52,36})));
    Modelica.Mechanics.MultiBody.Parts.FixedTranslation fixedTranslation(r={0,-1,
          0}) annotation (Placement(transformation(
          extent={{-10,-10},{10,10}},
          rotation=270,
          origin={52,0})));
    Modelica.Mechanics.MultiBody.Parts.Body body(m=500) annotation (Placement(
          transformation(
          extent={{-10,-11},{10,11}},
          rotation=270,
          origin={52,-29})));
    Modelica.Blocks.Continuous.Integrator integrator
      annotation (Placement(transformation(extent={{-40,0},{-20,20}})));
    Modelica.Blocks.Interfaces.RealInput inputValue
      annotation (Placement(transformation(extent={{-98,-10},{-58,30}})));
  equation
    connect(world.frame_b,prismatic. frame_a) annotation (Line(
        points={{-50,60},{-20,60}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(bodyShape.frame_a,prismatic. frame_b) annotation (Line(
        points={{28,60},{0,60}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(prismatic.support,position. support) annotation (Line(
        points={{-14,66},{-14,78}},
        color={0,127,0},
        smooth=Smooth.None));
    connect(position.flange,prismatic. axis) annotation (Line(
        points={{-4,88},{-4,66},{-2,66}},
        color={0,127,0},
        smooth=Smooth.None));
    connect(bodyShape.frame_b,revolute. frame_a) annotation (Line(
        points={{48,60},{52,60},{52,46}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(revolute.frame_b,fixedTranslation. frame_a) annotation (Line(
        points={{52,26},{52,10}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(fixedTranslation.frame_b,body. frame_a) annotation (Line(
        points={{52,-10},{52,-10},{52,-20},{52,-20},{52,-20},{52,-19}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(integrator.y, position.s_ref) annotation (Line(
        points={{-19,10},{2,10},{2,40},{-90,40},{-90,88},{-26,88}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(integrator.u, inputValue) annotation (Line(
        points={{-42,10},{-78,10}},
        color={0,0,127},
        smooth=Smooth.None));
    annotation (Diagram(coordinateSystem(preserveAspectRatio=false, extent={{
              -100,-100},{100,100}}), graphics));
  end crane_input;

  model crane_keyboard
    inner Modelica.Mechanics.MultiBody.World world
      annotation (Placement(transformation(extent={{-70,50},{-50,70}})));
    Modelica.Mechanics.MultiBody.Parts.BodyShape bodyShape(m=1, r={0,0,0})
      annotation (Placement(transformation(extent={{28,50},{48,70}})));
    Modelica.Mechanics.MultiBody.Joints.Prismatic prismatic(useAxisFlange=true)
      annotation (Placement(transformation(extent={{-20,50},{0,70}})));
    Modelica.Mechanics.Translational.Sources.Position position(useSupport=true)
      annotation (Placement(transformation(extent={{-24,78},{-4,98}})));
    Modelica.Mechanics.MultiBody.Joints.Revolute revolute(cylinderLength=0.2)
      annotation (Placement(transformation(
          extent={{-10,-10},{10,10}},
          rotation=270,
          origin={52,36})));
    Modelica.Mechanics.MultiBody.Parts.FixedTranslation fixedTranslation(r={0,-1,
          0}) annotation (Placement(transformation(
          extent={{-10,-10},{10,10}},
          rotation=270,
          origin={52,0})));
    Modelica.Mechanics.MultiBody.Parts.Body body(m=500) annotation (Placement(
          transformation(
          extent={{-10,-11},{10,11}},
          rotation=270,
          origin={52,-29})));
    Modelica_DeviceDrivers.Blocks.OperatingSystem.SynchronizeRealtime
                                        synchronizeRealtime
      annotation (Placement(transformation(extent={{-102,4},{-82,24}})));
    Modelica_DeviceDrivers.Blocks.InputDevices.KeyboardInput
                               keyboardInput
      annotation (Placement(transformation(extent={{-144,-38},{-124,-18}})));
    Modelica.Blocks.Logical.Switch switch1
      annotation (Placement(transformation(extent={{-56,-50},{-36,-30}})));
    Modelica.Blocks.Sources.Constant drive(k=1)
      annotation (Placement(transformation(extent={{-100,-30},{-80,-10}})));
    Modelica.Blocks.Sources.Constant no_drive(k=0)
      annotation (Placement(transformation(extent={{-100,-78},{-80,-58}})));
    Modelica.Blocks.Continuous.Integrator integrator
      annotation (Placement(transformation(extent={{-20,-50},{0,-30}})));
  equation
    connect(world.frame_b,prismatic. frame_a) annotation (Line(
        points={{-50,60},{-20,60}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(bodyShape.frame_a,prismatic. frame_b) annotation (Line(
        points={{28,60},{0,60}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(prismatic.support,position. support) annotation (Line(
        points={{-14,66},{-14,78}},
        color={0,127,0},
        smooth=Smooth.None));
    connect(position.flange,prismatic. axis) annotation (Line(
        points={{-4,88},{-4,66},{-2,66}},
        color={0,127,0},
        smooth=Smooth.None));
    connect(bodyShape.frame_b,revolute. frame_a) annotation (Line(
        points={{48,60},{52,60},{52,46}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(revolute.frame_b,fixedTranslation. frame_a) annotation (Line(
        points={{52,26},{52,10}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(fixedTranslation.frame_b,body. frame_a) annotation (Line(
        points={{52,-10},{52,-10},{52,-20},{52,-20},{52,-20},{52,-19}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(no_drive.y, switch1.u3) annotation (Line(
        points={{-79,-68},{-70,-68},{-70,-48},{-58,-48}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(keyboardInput.keyRight, switch1.u2) annotation (Line(
        points={{-128,-39},{-64,-39},{-64,-40},{-58,-40}},
        color={255,0,255},
        smooth=Smooth.None));
    connect(drive.y, switch1.u1) annotation (Line(
        points={{-79,-20},{-74,-20},{-74,-32},{-58,-32}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(switch1.y, integrator.u) annotation (Line(
        points={{-35,-40},{-22,-40}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(integrator.y, position.s_ref) annotation (Line(
        points={{1,-40},{2,-40},{2,40},{-90,40},{-90,88},{-26,88}},
        color={0,0,127},
        smooth=Smooth.None));
    annotation (Diagram(coordinateSystem(preserveAspectRatio=false, extent={{
              -100,-100},{100,100}}), graphics));
  end crane_keyboard;

  model crane_input2

    inner Modelica.Mechanics.MultiBody.World world
      annotation (Placement(transformation(extent={{-70,50},{-50,70}})));
    Modelica.Mechanics.MultiBody.Parts.BodyShape bodyShape(m=1, r={0,0,0})
      annotation (Placement(transformation(extent={{28,50},{48,70}})));
    Modelica.Mechanics.MultiBody.Joints.Prismatic prismatic(useAxisFlange=true)
      annotation (Placement(transformation(extent={{-20,50},{0,70}})));
    Modelica.Mechanics.Translational.Sources.Position position(useSupport=true)
      annotation (Placement(transformation(extent={{-24,78},{-4,98}})));
    Modelica.Mechanics.MultiBody.Joints.Revolute revolute(cylinderLength=0.2)
      annotation (Placement(transformation(
          extent={{-10,-10},{10,10}},
          rotation=270,
          origin={52,36})));
    Modelica.Mechanics.MultiBody.Parts.FixedTranslation fixedTranslation(r={0,-1,
          0}) annotation (Placement(transformation(
          extent={{-10,-10},{10,10}},
          rotation=270,
          origin={52,0})));
    Modelica.Mechanics.MultiBody.Parts.Body body(m=500) annotation (Placement(
          transformation(
          extent={{-10,-11},{10,11}},
          rotation=270,
          origin={52,-29})));
    Modelica.Blocks.Continuous.Integrator integrator
      annotation (Placement(transformation(extent={{-10,-10},{10,10}},
          rotation=90,
          origin={24,16})));
    Modelica.Blocks.Interfaces.RealInput inputValue(min=0.0, max=1.0)
      annotation (Placement(transformation(extent={{-120,-10},{-80,30}})));
    Modelica.Blocks.Interfaces.BooleanInput boolInput
      annotation (Placement(transformation(extent={{-120,-70},{-80,-30}})));
    Modelica.Blocks.Math.Add add
      annotation (Placement(transformation(extent={{0,-32},{20,-12}})));
    Modelica.Blocks.Logical.Switch switch1
      annotation (Placement(transformation(extent={{-38,-60},{-18,-40}})));
    Modelica.Blocks.Sources.RealExpression realExpression(y=0)
      annotation (Placement(transformation(extent={{-80,-86},{-60,-66}})));
    Modelica.Blocks.Sources.RealExpression realExpression1(y=-1)
      annotation (Placement(transformation(extent={{-80,-46},{-60,-26}})));
  equation
    connect(world.frame_b,prismatic. frame_a) annotation (Line(
        points={{-50,60},{-20,60}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(bodyShape.frame_a,prismatic. frame_b) annotation (Line(
        points={{28,60},{0,60}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(prismatic.support,position. support) annotation (Line(
        points={{-14,66},{-14,78}},
        color={0,127,0},
        smooth=Smooth.None));
    connect(position.flange,prismatic. axis) annotation (Line(
        points={{-4,88},{-4,66},{-2,66}},
        color={0,127,0},
        smooth=Smooth.None));
    connect(bodyShape.frame_b,revolute. frame_a) annotation (Line(
        points={{48,60},{52,60},{52,46}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(revolute.frame_b,fixedTranslation. frame_a) annotation (Line(
        points={{52,26},{52,10}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(fixedTranslation.frame_b,body. frame_a) annotation (Line(
        points={{52,-10},{52,-10},{52,-20},{52,-20},{52,-20},{52,-19}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(boolInput, switch1.u2) annotation (Line(
        points={{-100,-50},{-40,-50}},
        color={255,0,255},
        smooth=Smooth.None));
    connect(switch1.u3, realExpression.y) annotation (Line(
        points={{-40,-58},{-58,-58},{-58,-76},{-59,-76}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(realExpression1.y, switch1.u1) annotation (Line(
        points={{-59,-36},{-46,-36},{-46,-42},{-40,-42}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(inputValue, add.u1) annotation (Line(
        points={{-100,10},{-60,10},{-60,-16},{-2,-16}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(switch1.y, add.u2) annotation (Line(
        points={{-17,-50},{-8,-50},{-8,-28},{-2,-28}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(add.y, integrator.u) annotation (Line(
        points={{21,-22},{24,-22},{24,4}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(integrator.y, position.s_ref) annotation (Line(
        points={{24,27},{24,38},{-86,38},{-86,88},{-26,88}},
        color={0,0,127},
        smooth=Smooth.None));
    annotation (Diagram(coordinateSystem(preserveAspectRatio=false, extent={{-100,
              -100},{100,100}}),      graphics));
  end crane_input2;

  model crane_input3

    inner Modelica.Mechanics.MultiBody.World world
      annotation (Placement(transformation(extent={{-70,50},{-50,70}})));
    Modelica.Mechanics.MultiBody.Parts.BodyShape bodyShape(m=1, r={0,0,0})
      annotation (Placement(transformation(extent={{28,50},{48,70}})));
    Modelica.Mechanics.MultiBody.Joints.Prismatic prismatic(useAxisFlange=true)
      annotation (Placement(transformation(extent={{-20,50},{0,70}})));
    Modelica.Mechanics.MultiBody.Joints.Revolute revolute(cylinderLength=0.2)
      annotation (Placement(transformation(
          extent={{-10,-10},{10,10}},
          rotation=270,
          origin={52,36})));
    Modelica.Mechanics.MultiBody.Parts.FixedTranslation fixedTranslation(r={0,-1,
          0}) annotation (Placement(transformation(
          extent={{-10,-10},{10,10}},
          rotation=270,
          origin={52,0})));
    Modelica.Mechanics.MultiBody.Parts.Body body(m=500) annotation (Placement(
          transformation(
          extent={{-10,-11},{10,11}},
          rotation=270,
          origin={52,-29})));
    Modelica.Blocks.Interfaces.RealInput inputValue(min=0.0, max=1.0)
      annotation (Placement(transformation(extent={{-122,-32},{-82,8}})));
    Modelica.Mechanics.Translational.Sources.Speed speed(useSupport=true)
      annotation (Placement(transformation(extent={{-24,78},{-4,98}})));
  equation
    connect(world.frame_b,prismatic. frame_a) annotation (Line(
        points={{-50,60},{-20,60}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(bodyShape.frame_a,prismatic. frame_b) annotation (Line(
        points={{28,60},{0,60}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(bodyShape.frame_b,revolute. frame_a) annotation (Line(
        points={{48,60},{52,60},{52,46}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(revolute.frame_b,fixedTranslation. frame_a) annotation (Line(
        points={{52,26},{52,10}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(fixedTranslation.frame_b,body. frame_a) annotation (Line(
        points={{52,-10},{52,-10},{52,-20},{52,-20},{52,-20},{52,-19}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(speed.flange, prismatic.axis) annotation (Line(
        points={{-4,88},{-4,66},{-2,66}},
        color={0,127,0},
        smooth=Smooth.None));
    connect(speed.support, prismatic.support) annotation (Line(
        points={{-14,78},{-14,66}},
        color={0,0,0},
        pattern=LinePattern.None,
        smooth=Smooth.None));
    connect(inputValue, speed.v_ref) annotation (Line(
        points={{-102,-12},{-64,-12},{-64,14},{-88,14},{-88,86},{-26,86},{-26,
            88}},
        color={0,0,127},
        smooth=Smooth.None));
    annotation (Diagram(coordinateSystem(preserveAspectRatio=false, extent={{-100,
              -100},{100,100}}),      graphics));
  end crane_input3;

  model crane_input4

    inner Modelica.Mechanics.MultiBody.World world
      annotation (Placement(transformation(extent={{-70,50},{-50,70}})));
    Modelica.Mechanics.MultiBody.Parts.BodyShape bodyShape(m=1, r={0,0,0})
      annotation (Placement(transformation(extent={{28,50},{48,70}})));
    Modelica.Mechanics.MultiBody.Joints.Prismatic prismatic(useAxisFlange=true)
      annotation (Placement(transformation(extent={{-20,50},{0,70}})));
    Modelica.Mechanics.MultiBody.Joints.Revolute revolute(cylinderLength=0.2)
      annotation (Placement(transformation(
          extent={{-10,-10},{10,10}},
          rotation=270,
          origin={52,36})));
    Modelica.Mechanics.MultiBody.Parts.FixedTranslation fixedTranslation(r={0,-1,
          0}) annotation (Placement(transformation(
          extent={{-10,-10},{10,10}},
          rotation=270,
          origin={52,0})));
    Modelica.Mechanics.MultiBody.Parts.Body body(m=500) annotation (Placement(
          transformation(
          extent={{-10,-11},{10,11}},
          rotation=270,
          origin={52,-29})));
    Modelica.Mechanics.Translational.Sources.Speed speed(useSupport=true)
      annotation (Placement(transformation(extent={{-24,78},{-4,98}})));
    Modelica.Blocks.Interfaces.BooleanInput u(start=false)
      annotation (Placement(transformation(extent={{-126,-12},{-86,28}})));
    Modelica.Blocks.Logical.Switch switch1
      annotation (Placement(transformation(extent={{-54,-2},{-34,18}})));
    Modelica.Blocks.Sources.Constant const(k=0)
      annotation (Placement(transformation(extent={{-88,-22},{-68,-2}})));
    Modelica.Blocks.Sources.Constant const1(k=1)
      annotation (Placement(transformation(extent={{-88,18},{-68,38}})));
  equation
    connect(world.frame_b,prismatic. frame_a) annotation (Line(
        points={{-50,60},{-20,60}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(bodyShape.frame_a,prismatic. frame_b) annotation (Line(
        points={{28,60},{0,60}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(bodyShape.frame_b,revolute. frame_a) annotation (Line(
        points={{48,60},{52,60},{52,46}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(revolute.frame_b,fixedTranslation. frame_a) annotation (Line(
        points={{52,26},{52,10}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(fixedTranslation.frame_b,body. frame_a) annotation (Line(
        points={{52,-10},{52,-10},{52,-20},{52,-20},{52,-20},{52,-19}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(speed.flange, prismatic.axis) annotation (Line(
        points={{-4,88},{-4,66},{-2,66}},
        color={0,127,0},
        smooth=Smooth.None));
    connect(speed.support, prismatic.support) annotation (Line(
        points={{-14,78},{-14,66}},
        color={0,0,0},
        pattern=LinePattern.None,
        smooth=Smooth.None));
    connect(u, switch1.u2) annotation (Line(
        points={{-106,8},{-56,8}},
        color={255,0,255},
        smooth=Smooth.None));
    connect(switch1.y, speed.v_ref) annotation (Line(
        points={{-33,8},{-24,8},{-24,88},{-26,88}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(const.y, switch1.u3) annotation (Line(
        points={{-67,-12},{-58,-12},{-58,-6},{-56,-6},{-56,0}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(const1.y, switch1.u1) annotation (Line(
        points={{-67,28},{-66,28},{-66,16},{-56,16}},
        color={0,0,127},
        smooth=Smooth.None));
    annotation (Diagram(coordinateSystem(preserveAspectRatio=false, extent={{-100,
              -100},{100,100}}),      graphics));
  end crane_input4;

  model crane_input5

    inner Modelica.Mechanics.MultiBody.World world
      annotation (Placement(transformation(extent={{-24,46},{-4,66}})));
    Modelica.Mechanics.MultiBody.Parts.BodyShape bodyShape(m=1, r={0,0,0})
      annotation (Placement(transformation(extent={{62,46},{82,66}})));
    Modelica.Mechanics.MultiBody.Joints.Prismatic prismatic(useAxisFlange=true, v(fixed=
            false, start=0))
      annotation (Placement(transformation(extent={{26,46},{46,66}})));
    Modelica.Mechanics.MultiBody.Joints.Revolute revolute(cylinderLength=0.2,
        useAxisFlange=true)
      annotation (Placement(transformation(
          extent={{-10,-10},{10,10}},
          rotation=270,
          origin={88,38})));
    Modelica.Mechanics.MultiBody.Parts.FixedTranslation fixedTranslation(r={0,-1,
          0}) annotation (Placement(transformation(
          extent={{-10,-10},{10,10}},
          rotation=270,
          origin={88,12})));
    Modelica.Mechanics.MultiBody.Parts.Body body(m=50)  annotation (Placement(
          transformation(
          extent={{-10,-11},{10,11}},
          rotation=270,
          origin={88,-17})));
    Modelica.Mechanics.Translational.Sources.Speed speed(useSupport=true,
      s(fixed=true),
      exact=false,
      v(start=0))
      annotation (Placement(transformation(extent={{22,72},{42,92}})));
    Modelica.Blocks.Interfaces.BooleanInput u(start=false)
      annotation (Placement(transformation(extent={{-148,4},{-108,44}})));
    Modelica.Blocks.Math.Add add(k1=1, k2=1)
      annotation (Placement(transformation(extent={{-12,-10},{8,10}})));
    Modelica.Blocks.Interfaces.BooleanInput u1(
                                              start=false)
      annotation (Placement(transformation(extent={{-148,-42},{-108,-2}})));
    Modelica.Blocks.Logical.Switch switch1(y(start=0))
      annotation (Placement(transformation(extent={{-70,14},{-50,34}})));
    Modelica.Blocks.Sources.RealExpression realExpression(y=0)
      annotation (Placement(transformation(extent={{-100,6},{-80,26}})));
    Modelica.Blocks.Sources.RealExpression realExpression1(y=10)
      annotation (Placement(transformation(extent={{-100,22},{-80,42}})));
    Modelica.Blocks.Logical.Switch switch2(y(start=0))
      annotation (Placement(transformation(extent={{-68,-32},{-48,-12}})));
    Modelica.Blocks.Sources.RealExpression realExpression2(y=0)
      annotation (Placement(transformation(extent={{-100,-40},{-80,-20}})));
    Modelica.Blocks.Sources.RealExpression realExpression3(y=-10)
      annotation (Placement(transformation(extent={{-100,-24},{-80,-4}})));
    Modelica.Blocks.Continuous.Derivative derivative(k=2)
      annotation (Placement(transformation(extent={{16,-10},{36,10}})));
    Modelica.Blocks.Continuous.Integrator integrator
      annotation (Placement(transformation(extent={{-40,14},{-20,34}})));
    Modelica.Blocks.Continuous.Integrator integrator1
      annotation (Placement(transformation(extent={{-40,-32},{-20,-12}})));
    Modelica.Mechanics.Rotational.Components.Damper damper(d=10) annotation (
        Placement(transformation(
          extent={{-10,-10},{10,10}},
          rotation=90,
          origin={118,44})));
  equation
    connect(world.frame_b,prismatic. frame_a) annotation (Line(
        points={{-4,56},{26,56}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(bodyShape.frame_a,prismatic. frame_b) annotation (Line(
        points={{62,56},{46,56}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(bodyShape.frame_b,revolute. frame_a) annotation (Line(
        points={{82,56},{88,56},{88,48}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(revolute.frame_b,fixedTranslation. frame_a) annotation (Line(
        points={{88,28},{88,22}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(fixedTranslation.frame_b,body. frame_a) annotation (Line(
        points={{88,2},{88,-7}},
        color={95,95,95},
        thickness=0.5,
        smooth=Smooth.None));
    connect(speed.flange, prismatic.axis) annotation (Line(
        points={{42,82},{42,62},{44,62}},
        color={0,127,0},
        smooth=Smooth.None));
    connect(speed.support, prismatic.support) annotation (Line(
        points={{32,72},{32,62}},
        color={0,0,0},
        pattern=LinePattern.None,
        smooth=Smooth.None));
    connect(realExpression.y, switch1.u3) annotation (Line(
        points={{-79,16},{-72,16}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(realExpression1.y, switch1.u1) annotation (Line(
        points={{-79,32},{-72,32}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(u, switch1.u2) annotation (Line(
        points={{-128,24},{-72,24}},
        color={255,0,255},
        smooth=Smooth.None));
    connect(realExpression2.y, switch2.u3) annotation (Line(
        points={{-79,-30},{-70,-30}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(realExpression3.y, switch2.u1) annotation (Line(
        points={{-79,-14},{-70,-14}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(u1, switch2.u2) annotation (Line(
        points={{-128,-22},{-70,-22}},
        color={255,0,255},
        smooth=Smooth.None));
    connect(add.y, derivative.u) annotation (Line(
        points={{9,0},{14,0}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(switch1.y, integrator.u) annotation (Line(
        points={{-49,24},{-42,24}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(switch2.y, integrator1.u) annotation (Line(
        points={{-47,-22},{-42,-22}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(integrator1.y, add.u2) annotation (Line(
        points={{-19,-22},{-14,-22},{-14,-6}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(integrator.y, add.u1) annotation (Line(
        points={{-19,24},{-14,24},{-14,6}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(derivative.y, speed.v_ref) annotation (Line(
        points={{37,0},{40,0},{40,32},{8,32},{8,82},{20,82}},
        color={0,0,127},
        smooth=Smooth.None));
    connect(revolute.axis, damper.flange_a) annotation (Line(
        points={{98,38},{106,38},{106,34},{118,34}},
        color={0,0,0},
        smooth=Smooth.None));
    connect(damper.flange_b, revolute.support) annotation (Line(
        points={{118,54},{98,54},{98,44}},
        color={0,0,0},
        smooth=Smooth.None));
    annotation (Diagram(coordinateSystem(preserveAspectRatio=false, extent={{-100,
              -100},{100,100}}),      graphics));
  end crane_input5;
  annotation (uses(Modelica(version="3.2.1"), Modelica_DeviceDrivers(version=
            "1.2")));
end cranes;
