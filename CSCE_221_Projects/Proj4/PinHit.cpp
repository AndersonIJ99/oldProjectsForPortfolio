#include "PinHit.h"

PinHit::PinHit() {
    m_pin = -1;
    m_hits = 0;
}

PinHit::PinHit(int key, int value) {
    m_pin = key;
    m_hits = value;
}

void PinHit::SetKey(int pin) {
    this->m_pin = pin;
}

int PinHit::GetKey() const {
    return this->m_pin;
}

int PinHit::GetValue() const {
    return this->m_hits;
}

void PinHit::IncrementHits() {
    this->m_hits += 1;
}

int PinHit::CompareTo(const PinHit& other) const {
    // if number of hits is the same return 0
    if(this->m_hits == other.GetValue())
        return 0;
    
    // otherwise return the difference (positive or negative)
    return m_hits - other.GetValue();
}

bool PinHit::operator==(const PinHit& other) {
    if(this->m_pin == other.GetKey())
        return true;
    else
        return false;
}

