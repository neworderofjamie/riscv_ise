import pytest

from pyfenn.utils import is_kria

@pytest.fixture
def device(request):
    return request.param

def pytest_generate_tests(metafunc):
    if "device" in metafunc.fixturenames:
        device = [False]
        if is_kria():
            device.append(True)
        metafunc.parametrize("device", device, indirect=True)
