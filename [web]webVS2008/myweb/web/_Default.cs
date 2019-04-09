namespace web
{
    using System;
    using System.Web.UI;
    using System.Web.UI.WebControls;

    public class _Default : Page
    {
        protected CustomValidator CustomValidator1;

        private void InitializeComponent()
        {
            base.Load += new EventHandler(this.Page_Load);
        }

        protected override void OnInit(EventArgs e)
        {
            this.InitializeComponent();
            base.OnInit(e);
        }

        private void Page_Load(object sender, EventArgs e)
        {
            new system().loadConfig(0);
        }
    }
}
