#pragma once

namespace Time
{
	namespace Get
	{
		[[nodiscard]] float Elapsed(); // ���ӽ������κ��� ���� �ð�
		[[nodiscard]] float Delta(); // ��� ������Ʈ �Ǵ� �ð�
		[[nodiscard]] float NewDelta();
	}
};

