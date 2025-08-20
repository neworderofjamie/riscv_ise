import pytest

@pytest.fixture
def device(request):
    return request.param

def pytest_generate_tests(metafunc):
    if "device" in metafunc.fixturenames:
        metafunc.parametrize("device", [False], indirect=True)
