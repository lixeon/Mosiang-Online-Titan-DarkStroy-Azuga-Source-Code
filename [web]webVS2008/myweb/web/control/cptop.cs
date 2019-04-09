namespace web.control
{
    using System;
    using System.Web.UI;

    public class cptop : UserControl
    {
        private void btnlogout_Click(object sender, EventArgs e)
        {
            base.Session.Abandon();
            base.Response.Redirect("../default.aspx");
        }

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
        }
    }
}
