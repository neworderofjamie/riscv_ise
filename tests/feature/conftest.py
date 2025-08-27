import pytest

from os import uname

@pytest.fixture
def device(request):
    return request.param

def pytest_generate_tests(metafunc):
    if "device" in metafunc.fixturenames:
        device = [False]
        if uname().release.endswith("xilinx-zynqmp"):
            device.append(True)
        metafunc.parametrize("device", device, indirect=True)
