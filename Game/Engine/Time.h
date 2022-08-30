#pragma once

namespace Time
{
	namespace Get
	{
		[[nodiscard]] float Elapsed(); // 게임시작으로부터 지난 시간
		[[nodiscard]] float Delta(); // 계속 업데이트 되는 시간
		[[nodiscard]] float NewDelta();
	}
};

