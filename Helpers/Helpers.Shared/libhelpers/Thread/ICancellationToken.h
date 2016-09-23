#pragma once

namespace thread {
	class ICancellationToken {
	public:
		ICancellationToken();
		virtual ~ICancellationToken();

		virtual bool IsCancelled() const = 0;
		virtual void CancelTask() = 0;
	};
}