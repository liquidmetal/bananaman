import com.xtremelabs.robolectric.RobolectricTestRunner;
import com.xtremelabs.robolectric.RobolectricConfig;
import org.junit.runners.model.InitializationError;
import java.io.File;

public class TestRunner extends RobolectricTestRunner {
    public TestRunner(Class<?> testClass) throws InitializationError {
        super(testClass, new RobolectricConfig(new File("src/main")));
    }
}
