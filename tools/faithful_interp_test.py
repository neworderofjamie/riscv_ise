import matplotlib.pyplot as plt
import numpy as np

NUM_BITS = 15

# Split number of bits into 
TABLE_BIT = (NUM_BITS - 3) // 2
FRAC_BITS = TABLE_BIT + 3

FRAC_MASK = (1 << FRAC_BITS) - 1
FRAC_ONE = 2 ** FRAC_BITS
FRAC_HALF = 2 ** (FRAC_BITS - 1)

assert (TABLE_BIT + FRAC_BITS) == NUM_BITS

class FaithfulInterpolator:
    def __init__(self, range_start: float, range_end: float, 
                 value_fixed_point: int, function):
        self.range_start = range_start
        self.range_end = range_end

        # Generate range of x to generate table for
        assert self.range_end > self.range_start
        lut_range = np.arange(self.range_start, self.range_end, 
                              (self.range_end - self.range_start) / (2 ** TABLE_BIT))

        # Build LUT and check it will fit within the table bits
        func_fixed = lambda x: np.rint(function(x) * (2**value_fixed_point)).astype(int)
        self.lut = func_fixed(lut_range)
        self.lut = np.append(self.lut, func_fixed(self.range_end))

        assert np.all((self.lut >= 0) & (self.lut < (2**NUM_BITS)))


    def __call__(self, x: float):
        # Check in range
        assert np.all((x >= self.range_start) & (x < self.range_end))
        x_trunc = (x - self.range_start) / (self.range_end - self.range_start)

        # Convert to fixed point in fraction format
        x_fix = np.floor(x_trunc * (2**NUM_BITS)).astype(int)

        # Split
        ind = x_fix >> FRAC_BITS      # VSRLI
        frac = x_fix & FRAC_MASK      # VAND

        # Get LUT values to interpolate
        lut_start = self.lut[ind]
        lut_end = self.lut[ind + 1]
        
        # Linearly interpolate and round to nearest
        diff = lut_end - lut_start
        return lut_start + (((diff * frac) + FRAC_HALF) >> FRAC_BITS)

class Exponential(FaithfulInterpolator):
    log_2 = np.log(2)
    inv_log_2 = 1.0 / log_2
    exp_max = 0.5 * log_2
    
    def __init__(self):
        super().__init__(-self.exp_max, self.exp_max, 14,
                         lambda x: np.exp(x))
        
    def __call__(self, x: float):
        # Range reduce
        k = np.floor((x * self.inv_log_2) + 0.5).astype(int)
        r = x - (k * self.log_2)
        
        # Interpolate 
        exp_r = super().__call__(r)
        
        # Recombine
        # **TODO** exp_r in (0.7, 1.4) ish so can use 14 
        # fractional bits and multiply by whatever here
        return np.power(2.0, k) * exp_r

class Logarithm(FaithfulInterpolator):
    scale = np.rint(np.log(2) * (2**13)).astype(int)
    
    def __init__(self):
        super().__init__(1.0, 2.0, 13,
                         lambda x: np.log(x))

    def __call__(self, x: float):
        # Range reduce
        # 31 - CLZ
        k = np.floor(np.log2(x))
        
        # (x << FixedPoint) >> k
        r = x / np.power(2.0, k)
        
        # Interpolate 
        log_r = super().__call__(r)
        
        # Recombine
        return log_r + (k * self.scale)

class Reciprocal(FaithfulInterpolator):
    def __init__(self):
        super().__init__(1.0, 2.0, 14,
                         lambda x: 1.0 / x)
    
    def __call__(self, x: float):
        # Extract sign
        sign_x = np.sign(x)
        abs_x = np.abs(x)
        
        # Range reduce
        # 31 - CLZ
        k = np.floor(np.log2(abs_x))
        
        # (x << FixedPoint) >> k
        r = abs_x / np.power(2.0, k)
        
        # Interpolate
        rec_r = super().__call__(r)
        
        # Lookup table can use optimal number of bits and this can be scaled
        # **TODO** rec_r in (0.5, 1.0) ish so can use 14 
        # fractional bits and multiply by whatever here
        return sign_x * rec_r * np.power(2.0, -k)
        
# RECIPROCAL
# ============================================================================
reciprocal = Reciprocal()#LinearInterpolator(1.0, 2.0, False, 10, lambda x: 1.0 / x)
rec_x = np.append(np.arange(-10.0, -1 / 31.0, 0.001), np.arange(1 / 31.0, 10.0, 0.001))
rec_y_correct = np.rint((1.0 / rec_x) * 2**14)
rec_y_approx = reciprocal(rec_x)
rec_error = np.abs(rec_y_correct - rec_y_approx)

rec_fig, rec_axis = plt.subplots()
rec_fig.suptitle(f"Reciprocal: Max ULP={np.amax(rec_error)}, Average ULP={np.average(rec_error)}")
rec_axis.plot(rec_x, rec_y_correct, label="Correct")
rec_axis.plot(rec_x, rec_y_approx, label="Approx")[0]
rec_axis.axhline(np.iinfo(np.int16).max, linestyle="--", color="gray")
rec_axis.axhline(np.iinfo(np.int16).min, linestyle="--", color="gray")
rec_axis.legend()

# EXPONENTIAL
# ============================================================================
exponential = Exponential()
exp_x = np.arange(-3.4, 3.4, 0.0001)
exp_y_correct = np.rint(np.exp(exp_x) * 2**14)
exp_y_approx = exponential(exp_x)
exp_error = np.abs(exp_y_correct - exp_y_approx)
exp_fig, exp_axis = plt.subplots()
exp_fig.suptitle(f"Exponential: Max ULP={np.amax(exp_error)}, Average ULP={np.average(exp_error)}")
exp_axis.plot(exp_x, exp_y_correct, label="Correct")
exp_axis.plot(exp_x, exp_y_approx, label="Approx")[0]
exp_axis.axhline(np.iinfo(np.int16).max, linestyle="--", color="gray")
exp_axis.axhline(np.iinfo(np.int16).min, linestyle="--", color="gray")
exp_axis.legend()

# LOG
# ============================================================================
log = Logarithm()
log_x = np.arange(0.0001, 10.0, 0.0001)
log_y_correct = np.rint(np.log(log_x) * 2**13)
log_y_approx = log(log_x)
log_error = np.abs(log_y_correct - log_y_approx)

log_fig, log_axis = plt.subplots()
log_fig.suptitle(f"Natural logarithm: Max ULP={np.amax(log_error)}, Average ULP={np.average(log_error)}")
log_axis.plot(log_x, log_y_correct, label="Correct")
log_axis.plot(log_x, log_y_approx, label="Approx")[0]
log_axis.axhline(np.iinfo(np.int16).max, linestyle="--", color="gray")
log_axis.axhline(np.iinfo(np.int16).min, linestyle="--", color="gray")
log_axis.legend()

plt.show()
