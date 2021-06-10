#pragma once
namespace SB {

	class AppState
	{
	public:
		virtual void Initialize() {}
		virtual void Terminate() {}

		virtual void Update(float deltaTime) {}
		virtual void Render() {}
		virtual void DebugUI() {}
	};
}//namespace SB