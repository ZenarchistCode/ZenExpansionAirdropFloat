modded class ExpansionAirdropContainerBase
{
	#ifdef SERVER
	override void Expansion_OnUpdate(float dt)
	{
		super.Expansion_OnUpdate(dt);

		if (Zen_ShouldFloatOnWater())
		{
			Zen_ApplyBuoyancy(dt);
		}
	}

	float Zen_GetWaterDepth()
	{
		return GetGame().GetWaterDepth(GetPosition())
	}

	bool Zen_ShouldFloatOnWater()
	{
		vector pos = GetPosition();
		float depth = GetGame().GetWaterDepth(pos);
		float terrainY = GetGame().SurfaceY(pos[0], pos[2]);
		float distanceToBottom = pos[1] - terrainY;
		return depth > 0.2 && distanceToBottom > 0.5;
	}

	bool Zen_ShouldDelete()
	{
		return Zen_GetWaterDepth() > 20;
	}

	void Zen_ApplyBuoyancy(float dt)
	{
		vector impulse;
		vector pos = GetPosition();
		vector velocity = GetVelocity(this);
		velocity[1] = velocity[1] * 0.9; // dampen vertical motion

		float depth = GetGame().GetWaterDepth(pos);

		// Begin buoyant push as soon as we're submerged
		float submersion = Math.Clamp(depth / 1.0, 0.0, 1.0);
		float forceFactor = Zen_SmoothStep(0.0, 1.0, submersion);

		if (forceFactor <= 0)
		{
			// Apply gentle tickle to keep physics alive
			impulse = Vector(0, 0.05, 0);
			dBodyApplyImpulse(this, impulse);
			return;
		}

		// Zero lateral velocity
		velocity[0] = 0;
		velocity[2] = 0;
		SetVelocity(this, velocity);

		float sinkMultiplier = 1.0;
		if (m_Expansion_IsLooted && IsEmpty())
		{
			// Keep 20% buoyancy to make it sink slowly - only after it has been fully looted/emptied
			sinkMultiplier = 0.2;  

			if (Zen_ShouldDelete())
			{
				DeleteSafe();
				return;
			}
		}

		// Core buoyancy force
		float mass = dBodyGetMass(this);

		if (mass == 0 && !m_Expansion_IsLooted && IsEmpty()) // Dynamic physics disabled - re-enable it if not looted & on water
		{
			Expansion_CreateDynamicPhysics();
			return;
		}

		static const float gravity = 9.81;
		float buoyancyForce = gravity * forceFactor * 1.2 * sinkMultiplier; // *1.2 to overpower sink

		// Vertical velocity dampening
		buoyancyForce -= velocity[1] * 0.7;

		// Wave-like motion
		float wave = Math.Sin(GetGame().GetTime() * 0.001) * 0.1;
		buoyancyForce += wave;

		// Water-line tracking correction
		float surfaceY = GetGame().SurfaceY(pos[0], pos[2]);
		float waterY = surfaceY + depth;
		float desiredY = waterY + 0.5;

		float positionError = desiredY - pos[1];
		float correctionForce = Math.Clamp(positionError * 2.0, -1.5, 1.5);
		buoyancyForce += correctionForce;

		if (buoyancyForce < 0.05)
			buoyancyForce = 0.05;

		// Stabilize pitch/roll
		vector angles = GetAngles();
		angles[0] = 0;
		angles[2] = 0;
		SetAngles(angles);

		// Apply force upward
		vector transform[4];
		GetTransform(transform);
		impulse = Vector(0, buoyancyForce, 0) * mass * dt;
		vector impulseWS = impulse.Multiply3(transform);
		impulseWS[1] = Math.Clamp(impulseWS[1], -20, 25.2);

		dBodyApplyImpulse(this, impulseWS);
	}

	float Zen_SmoothStep(float edge0, float edge1, float x)
	{
		x = Math.Clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
		return x * x * (3.0 - 2.0 * x);
	}

	override private bool Expansion_CheckLanded()
	{
		if (!super.Expansion_CheckLanded())
		{
			if (Zen_ShouldFloatOnWater())
			{
				return true;
			}

			return false;
		}

		return true;
	}
	#endif
}