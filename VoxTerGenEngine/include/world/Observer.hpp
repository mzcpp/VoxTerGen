#ifndef OBSERVER_HPP
#define OBSERVER_HPP

class Observer
{
private:
	bool noclip_;

public:

	// Getters
	bool Noclip() const noexcept { return noclip_; }
	
	// Setters
	void SetNoclip(bool noclip) noexcept { noclip_ = noclip; }
};

#endif // OBSERVER_HPP

